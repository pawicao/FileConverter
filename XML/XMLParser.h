//
// Created by filja on 22.05.2020.
//

#ifndef FILECONVERTER_XMLPARSER_H
#define FILECONVERTER_XMLPARSER_H


#include "../document/Element.h"
#include "../document/Document.h"
#include "XMLLexer.h"

namespace XML
{
    class XMLParser
    {
    public:
        XMLParser(const std::shared_ptr<XMLLexer> &mLexer);

        Document parse();

        std::vector<ElementPtr> parseElements();

        ElementPtr parseElement();

        static void checkTokenType(const XMLToken& token, XMLTokenValue expected);
        XMLToken ReadCheckedToken(XMLTokenValue expected);
        XMLToken ReadToken();

    private:
        std::shared_ptr<XMLLexer> m_lexer;
        std::list<XMLToken> m_lookahead;

    };
}


#endif //FILECONVERTER_XMLPARSER_H
