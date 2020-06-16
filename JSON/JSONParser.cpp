//
// Created by joanna on 24.05.2020.
//

#include <iostream>
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
                token = readToken();
                if(token.token == COMMA){
                    return element;
                }
                if(token.token==RIGHT_BRACKET || token.token == RIGHT_BRACE)
                {
                    m_lookahead.push_back(token);
                    return element;
                }
                if(token.token != RIGHT_BRACE && token.token != RIGHT_BRACKET){
                    throw std::runtime_error("Unexpected token type after value in arr");
                }
            }else if(token.token == LEFT_BRACE) {
                element->setType(2);
                std::vector<ElementPtr> children = parseElements(false);
                element->setChildren(children);
                token = readToken();
                if(token.token == COMMA){
                    return element;
                }
                if(token.token==RIGHT_BRACKET || token.token == RIGHT_BRACE)
                {
                    m_lookahead.push_back(token);
                    return element;
                }
                if(token.token != RIGHT_BRACE && token.token != RIGHT_BRACKET && token.token != INVALID_TYPE){
                    throw std::runtime_error("Unexpected token type after object in arr");
                }
            }else if(token.token == LEFT_BRACKET) {
                element->setType(1);
                std::vector<ElementPtr> children = parseElements(true);
                element->setChildren(children);
                token = readToken();
                if(token.token == COMMA){
                    return element;
                }
                if(token.token==RIGHT_BRACKET || token.token == RIGHT_BRACE)
                {
                    m_lookahead.push_back(token);
                    return element;
                }
                if(token.token != RIGHT_BRACE && token.token != RIGHT_BRACKET && token.token != INVALID_TYPE){
                    throw std::runtime_error("Unexpected token type after array in arr");
                }
            }else if(token.token == INVALID_TYPE || token.token == RIGHT_BRACKET) {
                return std::shared_ptr<Element>();
            }else {
                throw std::runtime_error("Unexpected token type");
            }
        }

        if(token.token == INVALID_TYPE) {
            return std::shared_ptr<Element>();
        }

        if(token.token == LEFT_BRACE) {
            element->setType(2);
            std::vector<ElementPtr> children = parseElements(false);
            element->setChildren(children);
            return element;
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
                    token = readToken();
                    if(token.token == COMMA || token.token == RIGHT_BRACE || token.token == RIGHT_BRACKET){
                        return element;
                    }
                    if(token.token != RIGHT_BRACE && token.token != RIGHT_BRACKET){
                        throw std::runtime_error("Unexpected token type after value");
                    }

                }else if(token.token == LEFT_BRACE) {
                    element->setType(2);
                    std::vector<ElementPtr> children = parseElements(false);
                    element->setChildren(children);
                    token = readToken();
                    if(token.token == COMMA || token.token == RIGHT_BRACKET || token.token==RIGHT_BRACE){
                        return element;
                    }
//                    std::cout<<(token.token==INVALID_TYPE ? "invalid type" : "other")<<std::endl;
                    if(token.token != RIGHT_BRACE && token.token != RIGHT_BRACKET && token.token != INVALID_TYPE){
                        throw std::runtime_error("Unexpected token type after object");
                    }

                }else if(token.token == LEFT_BRACKET) {
                    element->setType(1);
                    std::vector<ElementPtr> children = parseElements(true);
                    element->setChildren(children);
                    token = readToken();
                    if(token.token == COMMA || token.token == RIGHT_BRACKET || token.token==RIGHT_BRACE){
                        return element;
                    }
                    if(token.token != RIGHT_BRACE && token.token != RIGHT_BRACKET && token.token != INVALID_TYPE){
                        throw std::runtime_error("Unexpected token type after array");
                    }
                }else {
                    throw std::runtime_error("Unexpected token type");
                }
            }else {
                throw std::runtime_error("Unexpected token type");
            }
        }
        m_lookahead.push_back(token);
        return std::shared_ptr<Element>();
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
