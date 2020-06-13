//
// Created by Oskar on 10.06.2020.
//

#ifndef FILECONVERTER_YAMLPARSER_H
#define FILECONVERTER_YAMLPARSER_H

#include "YAMLLexer.h"
#include "../document/Element.h"
#include "../document/Document.h"

namespace YAML {
    class YAMLParser {
    public:
        YAMLParser(const std::shared_ptr<YAMLLexer> &lexer);

        Document parse();

        ElementPtr handleComment();

        ElementPtr handleComment(ElementPtr & elementPtr);

        std::vector<ElementPtr> parseElements(const int & parent_indentation);

        ElementPtr parseElement(const int & parent_indentation);

        YAMLToken readToken();

    private:
        std::shared_ptr<YAMLLexer> lexer;
        std::list<YAMLToken> lookahead;
        bool last_seq_mode = NULL;
        int last_indentation = NULL;
        bool last_had_value = false;
    };
}


#endif //FILECONVERTER_YAMLPARSER_H
