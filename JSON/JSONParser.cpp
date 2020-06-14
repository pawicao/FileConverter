//
// Created by joanna on 24.05.2020.
//

#include "JSONParser.h"

namespace JSON
{
    JSONParser::JSONParser(const std::shared_ptr<JSONLexer> &mLexer) {
        this->m_lexer = mLexer;
    }

    Document JSONParser::parse() {
        std::vector<ElementPtr> topLevelElements = parseElements(false);
        Document document;
        for(auto& el: topLevelElements){
            document.addNewElement(*el);
        }
        return document;
    }

    std::vector<ElementPtr> JSONParser::parseElements(bool isArray) {
        std::vector<ElementPtr> elements;
        ElementPtr element;

        while(element = parseElement(isArray)){
            elements.push_back(element);
        }
        return elements;
    }

    ElementPtr JSONParser::parseElement(bool isArray) {
        JSONToken token;
        std::string element_value;
        std::string element_name;
        std::string tmp_name;
        token = readToken();

        ElementPtr element(new Element());

        if(isArray) {
            element->setName("item");
            if(token.token == STRING || token.token == NUMBER || token.token == JNULL || token.token == TRUE || token.token == FALSE) {
                element->setHasValue(true);
                element->setValue(token.value);
                readCheckedToken(COMMA);
            }else if(token.token == LEFT_BRACE) {
                std::vector<ElementPtr> children = parseElements(false);
                element->setChildren(children);
                readCheckedToken(COMMA);
            }else if(token.token == LEFT_BRACKET) {
                std::vector<ElementPtr> children = parseElements(true);
                element->setChildren(children);
                readCheckedToken(COMMA);
            }else {
                throw std::runtime_error("Unexpected token type");
            }
        }

        if(token.token == INVALID_TYPE) {
            return std::shared_ptr<Element>();
        }

        if(token.token == LEFT_BRACE) {
            std::vector<ElementPtr> children = parseElements(false);
            element->setChildren(children);
        }

        if(token.token == STRING) {
            tmp_name = token.value;
            token = readToken();
            if(token.token == COLON) {
                element->setName(tmp_name);
                token = readToken();
                if(token.token == STRING || token.token == NUMBER || token.token == JNULL || token.token == TRUE || token.token == FALSE) {
                    element->setHasValue(true);
                    element->setValue(token.value);
                    readCheckedToken(COMMA);
                }else if(token.token == LEFT_BRACE) {
                    std::vector<ElementPtr> children = parseElements(false);
                    element->setChildren(children);
                    readCheckedToken(COMMA);
                }else if(token.token == LEFT_BRACKET) {
                    std::vector<ElementPtr> children = parseElements(true);
                    element->setChildren(children);
                    readCheckedToken(COMMA);
                }else {
                    throw std::runtime_error("Unexpected token type");
                }
            }else {
                throw std::runtime_error("Unexpected token type");
            }
        }
        return element;
    }

    void JSONParser::checkToken(const JSONToken &token, JSONTokenType expectedType) {
        if(token.token != expectedType){
            throw std::runtime_error("Unexpected token type");
        }
    }

    JSONToken JSONParser::readCheckedToken(JSONTokenType expectedType) {
        JSONToken token = readToken();
        checkToken(token, expectedType);
        return token;
    }

    JSONToken JSONParser::readToken() {
        if(m_lookahead.empty()){
            return m_lexer->readToken();
        }
        else{
            JSONToken token = m_lookahead.front();
            m_lookahead.pop_front();
            return token;
        }
    }




}
