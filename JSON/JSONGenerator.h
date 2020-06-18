//
// Created by joanna on 14.06.2020.
//

#ifndef FILECONVERTER_JSONGENERATOR_H
#define FILECONVERTER_JSONGENERATOR_H

#include <string>
#include "../document/Element.h"
#include "../document/Document.h"

namespace JSON{
    class JSONGenerator {

    private:
        std::string printKey(const Element &element);
        std::string printValue(const Element &element);
        std::string printOpeningTag(int type);
        std::string printClosingTag(int type);
        std::string printWithProperTabulation(const std::string& printable);
        std::string printAttributes(const Element &element);
        std::string printChildren(const Element &element);
        std::string printWithProperTabulationStart(const std::string &printable);
        std::string printWithProperTabulationEnd(const std::string &printable);


        int tabCount;
    public:
        JSONGenerator();

        std::string GenerateJSON(const Document &document);
        std::string Generate(const Element &Element);

    };
}



#endif //FILECONVERTER_JSONGENERATOR_H
