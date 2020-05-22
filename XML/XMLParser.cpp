//
// Created by filja on 22.05.2020.
//

#include "XMLParser.h"
namespace XML
{
    XMLParser::XMLParser(const std::shared_ptr<XMLLexer> &mLexer) : m_lexer(mLexer)
    {}
    Document XMLParser::parse()
    {
        std::vector<ElementPtr> topLevelElements = parseElements();
        Document document;
        for(auto& el: topLevelElements)
        {
            document.addNewElement(*el);
        }
        return document;
    }

    std::vector<ElementPtr> XMLParser::parseElements()
    {
        std::vector<ElementPtr> elements;
        ElementPtr element;
        while (element = parseElement())
        {
            elements.push_back(element);
        }
        return elements;
    }

    ElementPtr XMLParser::parseElement()
    {
        XMLToken token;
        token = ReadToken();

        if(token.token == XML_INVALID)
        {
            return std::shared_ptr<Element>();
        }
        if(token.token != XML_LBRACKET)
        {
            m_lookahead.push_back(token);
            return std::shared_ptr<Element>();
        }
        token = ReadCheckedToken(XML_ATTRIBUTE);

        ElementPtr element(new Element(token.value));
        token = ReadToken();

        while(token.token == XML_ATTRIBUTE)
        {
            std::string attribName = token.value;
            ReadCheckedToken(XML_EQUALS);
            token = ReadCheckedToken(XML_VALUE);
            std::string attribValue = token.value;
            element->addAttribute(attribName, attribValue);
            token = ReadToken();
        }
        switch (token.token)
        {
            case XML_RBRACKET:
            {
                std::vector<ElementPtr> children = parseElements();
                if(children.empty())
                {
                    token = ReadCheckedToken(XML_ATTRIBUTE);
                    element->setHasValue(true);
                    element->setValue(token.value);
                }
                else
                {
                    element->setChildren(children);
                }

                ReadCheckedToken(XML_LSLASH);
                token = ReadCheckedToken(XML_ATTRIBUTE);
                if(token.value != element->getName())
                {
                    throw std::runtime_error("MIsmatched element tags: expected " + element->getName() + " not "+ token.value);

                }
                ReadCheckedToken(XML_RBRACKET);
                break;
            }
            case XML_RSLASH:
            {
                break;
            }
            default:
            {
                throw std::runtime_error("Unexpected token type");
            }
        }
        return element;
    }

    void XMLParser::checkTokenType(const XMLToken &token, XMLTokenValue expected)
    {
        if(token.token != expected)
        {
            throw std::runtime_error("Unexpected token type");
        }
    }

    XMLToken XMLParser::ReadCheckedToken(XMLTokenValue expected)
    {
        XMLToken token = ReadToken();
        checkTokenType(token, expected);
        return token;
    }

    XMLToken XMLParser::ReadToken()
    {
        if(m_lookahead.empty())
        {
            return m_lexer->nextToken();
        }
        else
        {
            XMLToken token = m_lookahead.front();
            m_lookahead.pop_front();
            return token;
        }
    }
}