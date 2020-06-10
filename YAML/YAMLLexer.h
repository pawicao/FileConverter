//
// Created by Oskar on 22.05.2020.
//

#ifndef FILECONVERTER_YAMLLEXER_H
#define FILECONVERTER_YAMLLEXER_H

#include <list>
#include <fstream>
#include "YAMLToken.h"

namespace YAML {
    class YAMLLexer {
    public:
        YAMLLexer(const std::string &filename);

        YAMLToken nextToken();

        unsigned char nextChar();

    private:
        YAMLState state;
        bool eof = false;
        std::ifstream is;
        std::list<unsigned char> lookahead;

    };
}

#endif //FILECONVERTER_YAMLLEXER_H
