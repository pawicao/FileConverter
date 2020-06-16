//
// Created by filja on 21.05.2020.
//
#ifndef FILECONVERTER_XMLTOKENS_H
#define FILECONVERTER_XMLTOKENS_H
#include <string>
#include <memory>
namespace XML
{
    const static std::string ROOT = "../";
    enum XMLTokenValue {
        XML_EQUALS,
        XML_ATTRIBUTE,
        XML_LBRACKET,
        XML_LSLASH,
        XML_RBRACKET,
        XML_RSLASH,
        XML_VALUE,
        XML_INVALID
    };

    enum XMLState {
        START_STATE,
        EOF_STATE,
        EQUALS_STATE,
        HALF_RSLASH_STATE,
        HALF_VALUE_STATE,
        ATTRIBUTE_STATE,
        LBRACKET_STATE,
        RBRACKET_STATE,
        LSLASH_STATE,
        RSLASH_STATE,
        VALUE_STATE,
        INVALID_STATE

    };


    class XMLToken {

    public:
        XMLToken() : token(XML_INVALID), value("") {}

        XMLToken(XMLTokenValue token, const std::string &value) : token(token), value(value) {}

        XMLTokenValue token;
        std::string value;
    };
}


#endif //FILECONVERTER_XMLTOKENS_H
