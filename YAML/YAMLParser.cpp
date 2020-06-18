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
            if(parent_indentation == -1 && last_seq_mode) {
                ElementPtr el_to_add(new Element("root"));
                if(element->getChildren().empty()) {
                    if(element->hasValueFun()) {
                        ElementPtr child(new Element(element->getName(), element->getValue()));
                        el_to_add->addChild(child);
                    }
                    else {
                        el_to_add->setValue(element->getName());
                    }
                }
                else {
                    ElementPtr child(new Element(element->getName()));
                    child->setChildren(element->getChildren());
                    el_to_add->addChild(child);
                }
                elements.push_back(el_to_add);
            }
            else if(!element->getChildren().empty() && element->getChildren()[0]->getName().substr(0,3) == "`'`") {
                for(auto & el : element->getChildren()) {
                    el->setName(el->getName().substr(3, el->getName().size()));
                    ElementPtr el_to_add(new Element(element->getName()));
                    if(el->getChildren().empty()) {
                        if(el->hasValueFun()) {
                            el_to_add->addChild(el);
                        }
                        else {
                            el_to_add->setValue(el->getName());
                        }
                    }
                    else {
                        ElementPtr child(new Element(el->getName()));
                        child->setChildren(el->getChildren());
                        el_to_add->addChild(child);
                    }
                    elements.push_back(el_to_add);
                }
            }
            else {
                elements.push_back(element);
            }
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
        if(last_seq_mode) {
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
                element_name = "`'`";
                element_name += "null";
                ElementPtr element(new Element(element_name));
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
                    element_name = "`'`";
                    element_name += "null";
                    ElementPtr element(new Element(element_name));
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
        if(seq_mode) {
            element_name = "`'`";
            element_name += token.value;
        }
        else {
            element_name = token.value;
        }
        ElementPtr element(new Element(element_name));
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
        }
        else if (token.token == FOLDED_BLOCK_TOKEN || token.token == LITERAL_BLOCK_TOKEN){
            bool endline_present = true;
            element_value = "";
            bool folded = token.token == FOLDED_BLOCK_TOKEN;
            unsigned int indentCount;
            token = readToken();
            if(token.token == HYPHEN_TOKEN) {
                endline_present = false;
                token = readToken();
            }
            while(token.token == INDENT_TOKEN) {
                token = readToken();
            }
            if(token.token != EOL_TOKEN) {
                throw std::runtime_error("There must be a newline after the start of a block scalar.");
            }
            while(true) {
                indentCount = 0;
                while(true) {
                    token = readToken();
                    if(token.token == INDENT_TOKEN) {
                        ++indentCount;
                    }
                    else {
                        break;
                    }
                }
                if(indentCount <= indentation_level) {
                    for(size_t i = 0; i < indentCount; ++i) {
                        lookahead.emplace_back(INDENT_TOKEN, "");
                    }
                    lookahead.push_back(token);
                    if(!endline_present && !element_value.empty() && element_value.back() == '\n') {
                        element_value = element_value.substr(0, element_value.size()-1);
                    }
                    break;
                }
                else {
                    if(token.token == QUOTED_SCALAR_TOKEN) {
                        element_value+= '"';
                        element_value+= token.value;
                        element_value+= '"';
                        if(folded)
                            element_value+= ' ';
                        else
                            element_value+= '\n';
                    }
                    else if(token.token == YNULL_TOKEN || token.token == TRUE_TOKEN || token.token == FALSE_TOKEN ||
                            token.token == PLAIN_SCALAR_TOKEN) {
                        element_value += token.value;
                        if(folded)
                            element_value+= ' ';
                        else
                            element_value+= '\n';
                    }
                    else if(token.token == INVALID_TYPE_TOKEN || token.token == EOL_TOKEN) {
                        if(folded)
                            element_value+= ' ';
                        else
                            element_value+= '\n';
                        break;
                    }
                    else {
                        throw std::runtime_error("Extra-characters in block scalars not supported.");
                    }
                    token = readToken();
                    if(token.token != EOL_TOKEN && token.token != INVALID_TYPE_TOKEN) {
                        lookahead.push_back(token);
                    }
                }
            }
            element->setHasValue(true);
            element->setValue(element_value);
            last_had_value = true;
        }
        else if (token.token == YNULL_TOKEN || token.token == TRUE_TOKEN || token.token == FALSE_TOKEN ||
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