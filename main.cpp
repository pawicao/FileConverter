#include <iostream>
#include <memory>
#include "document/Document.h"
#include "XML/XMLLexer.h"
#include "XML/XMLParser.h"
#include "YAML/YAMLLexer.h"
#include "YAML/YAMLParser.h"

void printMeAndChildren(Element element, std::string preset) {
    preset += "  ";
    std::cout<<preset<<element.getName()<<": "<<element.getValue()<<std::endl;
    for(const std::shared_ptr<Element>& child : element.getChildren()) {
        printMeAndChildren(*child, preset);
    }
}

int main() {
    std::shared_ptr<XML::XMLLexer> lexerX(new XML::XMLLexer("test.xml"));
    XML::XMLParser parserX(lexerX);

    Document test = parserX.parse();
    for(auto & element : test.getElements()) {
        printMeAndChildren(element, "");
    }

    std::shared_ptr<YAML::YAMLLexer> lexerY(new YAML::YAMLLexer("test.yml"));
    YAML::YAMLParser parserY(lexerY);

    Document test2 = parserY.parse();
    for(auto & element : test2.getElements()) {
        printMeAndChildren(element, "");
    }

    return 0;
}
