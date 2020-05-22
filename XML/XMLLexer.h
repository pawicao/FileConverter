//
// Created by filja on 21.05.2020.
//

#ifndef FILECONVERTER_XMLLEXER_H
#define FILECONVERTER_XMLLEXER_H

#include <list>
#include <fstream>
#include "XML_tokens.h"
namespace XML
{
    class XMLLexer {
    public:
        XMLLexer(const std::string& filename);
        XMLToken nextToken();
        unsigned char nextChar();

    private:
        XMLState state;
        bool m_eof;
        std::ifstream m_is;
        std::list<unsigned char> m_lookahead;


    };
}


#endif //FILECONVERTER_XMLLEXER_H
