//
// Created by joanna on 24.05.2020.
//

#ifndef FILECONVERTER_JSONPARSER_H
#define FILECONVERTER_JSONPARSER_H

#include <bits/shared_ptr.h>
#include "../document/Element.h"
#include "JSONToken.h"

namespace JSON {
    class JSONParser {
    private:
        std::shared_ptr<JSONLexer> m_lexer;
        std::list<JSONToken> m_lookahead;

    public:
        std::vector<ElementPtr> parseElements();
        ElementPtr  parseElement();
        JSONParser(const std::shared_ptr<JSONLexer> &mLexer);
        static void checkToken(const JSONToken& token, JSONTokenType expectedType);
        JSONToken readCheckedToken(JSONTokenType expectedType);
        JSONToken readToken();


    };
}

#endif //FILECONVERTER_JSONPARSER_H
