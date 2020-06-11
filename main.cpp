#include <iostream>
#include <memory>
#include "document/Document.h"
#include "XML/XMLLexer.h"
#include "XML/XMLParser.h"
#include "YAML/YAMLLexer.h"
#include "YAML/YAMLParser.h"

int main() {
    std::shared_ptr<XML::XMLLexer> lexer(new XML::XMLLexer("test.xml"));
    XML::XMLParser parser(lexer);

    Document test = parser.parse();
    for(auto & element : test.elements) {
        std::cout<<element.getName()<<": "<<element.getValue()<<std::endl;
        for(auto & child : element.getChildren()) {
            std::cout<<" "<<child->getName()<<": "<<child->getValue()<<std::endl;
        }
    }

    std::shared_ptr<YAML::YAMLLexer> lexerY(new YAML::YAMLLexer("test.yml"));
    YAML::YAMLParser parserY(lexerY);

    Document test2 = parserY.parse();
    for(auto & element : test2.elements) {
        std::cout<<element.getName()<<": "<<element.getValue()<<std::endl;
        for(auto & child : element.getChildren()) {
            std::cout<<" "<<child->getName()<<": "<<child->getValue()<<std::endl;
        }
    }

    return 0;
}
