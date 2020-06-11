//
// Created by Oskar on 10.06.2020.
//

#include <iostream>
#include "YAMLParser.h"

namespace YAML {
    YAMLParser::YAMLParser(const std::shared_ptr<YAMLLexer> &lexerArg) : lexer(lexerArg) {}

    Document YAMLParser::parse() {
        std::vector<ElementPtr> topLevelElements = parseElements(-1);
        Document document;
        for (auto &el: topLevelElements) {
            document.addNewElement(*el);
        }
        return document;
    }

    std::vector<ElementPtr> YAMLParser::parseElements(const int & parent_indentation) {
        std::vector<ElementPtr> elements;
        ElementPtr element;
        while (element = parseElement(parent_indentation)) {
            elements.push_back(element);
        }
        return elements;
    }

    ElementPtr YAMLParser::handleComment() {
        YAMLToken token;
        do {
            token = readToken();
        } while(token.token != EOL_TOKEN && token.token != INVALID_TYPE_TOKEN);
        return std::shared_ptr<Element>();
    }

    ElementPtr YAMLParser::handleComment(ElementPtr & elementPtr) {
        YAMLToken token;
        do {
            token = readToken();
        } while(token.token != EOL_TOKEN && token.token != INVALID_TYPE_TOKEN);
        return elementPtr;
    }

    ElementPtr YAMLParser::parseElement(const int & parent_indentation) {
        int indentation_level = 0;
        YAMLToken token;
        YAMLToken returnToken;
        bool seq_mode = false;
        std::string element_value;
        std::string element_name;

        while(true) {
            token = readToken();
            if(token.token == INDENT_TOKEN) {
                ++indentation_level;
            }
            else if(token.token == EOL_TOKEN) {
                indentation_level = 0;
            }
            else {
                break;
            }
        }
        if(token.token == INVALID_TYPE_TOKEN) {
            return std::shared_ptr<Element>();
        }
        else if(token.token == COMMENT_TOKEN) {
            return handleComment();
        }
        if(indentation_level <= parent_indentation) {
            for(int i = 0; i < indentation_level; ++i) {
                lookahead.emplace_back(INDENT_TOKEN, "");
            }
            lookahead.push_back(token);
            return std::shared_ptr<Element>();
        }
        else if(indentation_level > last_indentation && last_had_value) {
            throw std::runtime_error("Cannot assign children to an element which already has a value.");
        }
        if(token.token == HYPHEN_TOKEN) {
            seq_mode = true;
            token = readToken();
            if(token.token != INDENT_TOKEN && token.token != EOL_TOKEN)
                throw std::runtime_error("Space needed between hyphen and key in sequences.");
        }
        if(last_seq_mode != NULL) {
            if(last_seq_mode != seq_mode && last_indentation == indentation_level) {
                if(seq_mode)
                    throw std::runtime_error("You cannot define a sequence item when in a mapping.");
                else
                    throw std::runtime_error("You cannot define a mapping item when in a sequence.");
            }
        }
        int hyphened_indent;
        if(token.token == EOL_TOKEN)
            hyphened_indent = 0;
        else
            hyphened_indent = -1;
        while(seq_mode) {
            token = readToken();
            if(token.token == EOL_TOKEN) {
                hyphened_indent = 0;
            }
            else if (token.token == INDENT_TOKEN) {
                ++hyphened_indent;
            }
            else if (token.token == COMMENT_TOKEN) {
                last_indentation = indentation_level;
                last_seq_mode = true;
                last_had_value = true;
                ElementPtr element(new Element("null"));
                return handleComment(element);
            }
            else {
                break;
            }
        }
        if(seq_mode) {
            if(hyphened_indent <= indentation_level && hyphened_indent != -1) {
                if(hyphened_indent == indentation_level && token.token == HYPHEN_TOKEN) {
                    for(int i = 0; i < hyphened_indent; ++i) {
                        lookahead.emplace_back(INDENT_TOKEN, "");
                    }
                    lookahead.push_back(token);
                    last_indentation = indentation_level;
                    last_seq_mode = true;
                    last_had_value = true;
                    ElementPtr element(new Element("null"));
                    return element;
                }
                throw std::runtime_error("Wrong indentation in a sequence.");
            }
        }
        if(token.token != YNULL_TOKEN && token.token != TRUE_TOKEN && token.token != FALSE_TOKEN &&
        token.token != PLAIN_SCALAR_TOKEN && token.token != QUOTED_SCALAR_TOKEN) {
            throw std::runtime_error("Extra-characters keys not supported.");
        }
        YAMLToken token2 = readToken();
        ElementPtr element(new Element(token.value));
        while(token2.token == INDENT_TOKEN) {
            token2 = readToken();
        }
        if(seq_mode) {
            if(token2.token == MAPPER_TOKEN) {}
            else if(token2.token != EOL_TOKEN && token2.token != INVALID_TYPE_TOKEN
            && token2.token != COMMENT_TOKEN) {
                throw std::runtime_error("Wrong token after key element of name: " + token.value);
            }
            else {
                last_seq_mode = seq_mode;
                last_indentation = indentation_level;
                last_had_value = true;
                if(token2.token == COMMENT_TOKEN) {
                    return handleComment(element);
                }
                return element;
            }
        }
        else {
            if(token2.token != MAPPER_TOKEN)
                throw std::runtime_error("Wrong token after key element of name: " + token.value);
        }
        do {
            token = readToken();
        } while(token.token == INDENT_TOKEN);
        if(token.token == EOL_TOKEN || token.token == INVALID_TYPE_TOKEN || token.token == COMMENT_TOKEN) {
            if(token.token == COMMENT_TOKEN) {
                do {
                    token = readToken();
                } while (token.token != EOL_TOKEN && token.token != INVALID_TYPE_TOKEN);
            }
            last_had_value = false;
            std::vector<ElementPtr> children = parseElements(indentation_level);
            if(children.empty()) {
                element->setHasValue(true);
                element->setValue("null");
            }
            else {
                element->setHasValue(false);
                element->setChildren(children);
            }
        } else if (token.token == YNULL_TOKEN || token.token == TRUE_TOKEN || token.token == FALSE_TOKEN ||
                   token.token == PLAIN_SCALAR_TOKEN || token.token == QUOTED_SCALAR_TOKEN) {
            element_value = token.value;
            if(token.token != QUOTED_SCALAR_TOKEN) {
                unsigned int indentCount = 0;
                bool commentMode = false;
                do {
                    token = readToken();
                    if(token.token == INDENT_TOKEN) indentCount++;
                    else if (token.token == COMMENT_TOKEN) commentMode = true;
                    else if(!commentMode) {
                        for(int i = 0; i < indentCount; ++i) {
                            element_value += ' ';
                        }
                        element_value += token.value;
                        indentCount = 0;
                    }
                } while(token.token != EOL_TOKEN && token.token != INVALID_TYPE_TOKEN);
                if(token.token != COMMENT_TOKEN) {
                    lookahead.push_back(token);
                }
            }
            element->setHasValue(true);
            element->setValue(element_value);
            last_had_value = true;
        }
        else {
            throw std::runtime_error("Extra-characters values not supported.");
        }
        last_seq_mode = seq_mode;
        last_indentation = indentation_level;
        return element;
    }

    YAMLToken YAMLParser::readToken()
    {
        if(lookahead.empty())
        {
            return lexer->nextToken();
        }
        else
        {
            YAMLToken token = lookahead.front();
            lookahead.pop_front();
            return token;
        }
    }
}