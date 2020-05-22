//
// Created by filja on 21.05.2020.
//

#include "XMLLexer.h"
#include <stdexcept>
#include <iostream>

namespace XML
{
    XMLToken XMLLexer::nextToken()
    {
        if (state == EOF_STATE)
        {
            return XMLToken();
        }
        state = START_STATE;
        std::string value;

        while (true)
        {
            switch (state)
            {
                case START_STATE:
                {
                    unsigned char c = nextChar();
                    if (m_eof)
                    {
                        state = EOF_STATE;
                    }
                    else if (c == '=')
                    {
                        state = EQUALS_STATE;
                    }
                    else if (c == '/')
                    {
                        state = HALF_RSLASH_STATE;
                    }
                    else if (c == '"')
                    {
                        state = HALF_VALUE_STATE;
                    }
                    else if (c == '<')
                    {
                        state = LBRACKET_STATE;
                    }
                    else if (c == '>')
                    {
                        state = RBRACKET_STATE;
                    }
                    else if (isalpha(c) || isdigit(c) || c == '.')
                    {
                        state = ATTRIBUTE_STATE;
                        value += c;
                    }
                    break;
                }
                case EOF_STATE:
                {
                    return XMLToken();
                }
                case EQUALS_STATE:
                {
                    return XMLToken(XML_EQUALS, "");
                }
                case HALF_RSLASH_STATE:
                {
                    unsigned char c = nextChar();
                    if (m_eof || c != '>')
                    {
                        state = INVALID_STATE;
                        value = ">";
                    }
                    else
                    {
                        state = RSLASH_STATE;
                    }
                    break;
                }
                case HALF_VALUE_STATE:
                {
                    unsigned char c = nextChar();
                    if (m_eof)
                    {
                        state = INVALID_STATE;
                        value = "\"";
                    }
                    else if (c == '"')
                    {
                        state = VALUE_STATE;
                    }
                    else
                    {
                        value += c;
                    }
                    break;
                }
                case ATTRIBUTE_STATE:
                {
                    unsigned char c = nextChar();
                    if(m_eof)
                    {
                        state = EOF_STATE;
                        return XMLToken(XML_ATTRIBUTE, value);
                    }
                    else if(isalpha(c) || isdigit(c) || c == '.' || c == '_')
                    {
                        value += c;
                    }
                    else
                    {
                        m_lookahead.push_back(c);
                        return XMLToken(XML_ATTRIBUTE, value);
                    }
                    break;
                }
                case LBRACKET_STATE:
                {
                    unsigned char c = nextChar();
                    if(m_eof)
                    {
                        state = EOF_STATE;
                        return XMLToken(XML_LBRACKET, "");
                    }
                    else if(c == '/')
                    {
                        state = LSLASH_STATE;
                    }
                    else
                    {
                        m_lookahead.push_back(c);
                        return XMLToken(XML_LBRACKET, "");
                    }
                    break;
                }
                case LSLASH_STATE:
                {
                    return XMLToken(XML_LSLASH, "");
                }
                case RSLASH_STATE:
                {
                    return XMLToken(XML_RSLASH, "");
                }
                case RBRACKET_STATE:
                {
                    return XMLToken(XML_RBRACKET, "");
                }
                case VALUE_STATE:
                {
                    return XMLToken(XML_VALUE, value);
                }
                default:
                {
                    throw std::runtime_error("Error: Expected" + value);
                }
            }
        }
    }

    unsigned char XMLLexer::nextChar()
    {
        if(m_lookahead.empty())
        {
            unsigned char c = m_is.get();
            if(m_is.eof())
            {
                m_eof = true;
            }
            return c;
        }
        else
        {
            unsigned char c = m_lookahead.front();
            m_lookahead.pop_front();
            m_eof = false;
            return c;
        }
    }

    XMLLexer::XMLLexer(const std::string& filename)
    {
        m_is = std::ifstream(ROOT+filename, std::ifstream::in);
    }
}
