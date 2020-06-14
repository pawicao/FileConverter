//
// Created by joanna on 24.05.2020.
//

#ifndef FILECONVERTER_JSONPARSER_H
#define FILECONVERTER_JSONPARSER_H

#include "../document/Document.h"
#include "../document/Element.h"
#include "JSONToken.h"
#include "JSONLexer.h"


namespace JSON {
    class JSONParser {
    private:
        std::shared_ptr<JSONLexer> m_lexer;
        std::list<JSONToken> m_lookahead;

    public:
        Document parse();
        std::vector<ElementPtr> parseElements(bool isArray);
        ElementPtr  parseElement(bool isArray);
        JSONParser(const std::shared_ptr<JSONLexer> &mLexer);
        static void checkToken(const JSONToken& token, JSONTokenType expectedType);
        JSONToken readCheckedToken(JSONTokenType expectedType);
        JSONToken readToken();


    };
}

#endif //FILECONVERTER_JSONPARSER_H
