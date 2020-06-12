#include <iostream>
#include <memory>
#include "document/Document.h"
#include "XML/XMLLexer.h"
#include "XML/XMLParser.h"
#include "YAML/YAMLLexer.h"
#include "YAML/YAMLParser.h"
#include "XML/XMLGenerator.h"

int main() {
    std::shared_ptr<XML::XMLLexer> lexer(new XML::XMLLexer(XML::ROOT+"test.xml"));
    XML::XMLParser parser(lexer);

    Document test = parser.parse();

    XML::XMLGenerator generator{};
    std::cout<<generator.GenerateXml(test);

    std::shared_ptr<YAML::YAMLLexer> lexerY(new YAML::YAMLLexer(XML::ROOT+"test.yml"));
    YAML::YAMLParser parserY(lexerY);

    Document test2 = parserY.parse();

    std::cout<<generator.GenerateXml(test2);
    return 0;
}
