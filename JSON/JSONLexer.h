//
// Created by joanna on 24.05.2020.
//

#ifndef FILECONVERTER_JSONLEXER_H
#define FILECONVERTER_JSONLEXER_H

#include <fstream>
#include <list>
#include "JSONToken.h"

namespace JSON
{
    class JSONLexer {
    private:
        JSONState jsonState;
        bool end;
        std::ifstream file;
        std::list<unsigned char> rest;


    public:
        JSONLexer(std::string path);
        JSONToken readToken();
        unsigned char readChar();

    };
}

#endif //FILECONVERTER_JSONLEXER_H
