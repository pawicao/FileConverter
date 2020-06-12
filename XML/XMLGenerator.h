//
// Created by filja on 12.06.2020.
//

#ifndef FILECONVERTER_XMLGENERATOR_H
#define FILECONVERTER_XMLGENERATOR_H


#include <string>
#include "../document/Element.h"
#include "../document/Document.h"

namespace XML
{
    class XMLGenerator
    {

    public:
        XMLGenerator();

        std::string GenerateXml(const Document &document);
        std::string Generate(const Element &Element);

    private:
        std::string printOpeningTag();

        std::string printAttributes(const Element &element);

        std::string printClosingTag(const Element &element);

        std::string printValue(const Element &element);

        std::string printChildren(const Element &element);

        std::string printEndingTag(const Element &element);

        std::string printWithProperTabulation(const std::string& printable);

        int tabCount = 0;

        const std::string &printName(const Element &element) const;
    };
}

#endif //FILECONVERTER_XMLGENERATOR_H
