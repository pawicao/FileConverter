#include <iostream>
#include <memory>
#include "document/Document.h"
#include "XML/XMLLexer.h"
#include "XML/XMLParser.h"
#include "YAML/YAMLLexer.h"
#include "YAML/YAMLParser.h"
#include "XML/XMLGenerator.h"
#include "YAML/YAMLGenerator.h"

int main() {
    XML::XMLGenerator xml_generator{};
    YAML::YAMLGenerator yaml_generator{};

    std::shared_ptr<XML::XMLLexer> xml_lexer(new XML::XMLLexer(XML::ROOT+"test.xml"));
    std::shared_ptr<YAML::YAMLLexer> yaml_lexer(new YAML::YAMLLexer(XML::ROOT+"test.yml"));

    XML::XMLParser xml_parser(xml_lexer);
    YAML::YAMLParser yaml_parser(yaml_lexer);

    Document test = xml_parser.parse();
    Document test2 = yaml_parser.parse();

    std::cout<<"\n------------\n";
    std::cout<<"-- XML 1: --\n";
    std::cout<<"------------\n";
    std::cout<<xml_generator.GenerateXml(test);
    std::cout<<"\n------------\n";
    std::cout<<"-- XML 2: --\n";
    std::cout<<"------------\n";
    std::cout<<xml_generator.GenerateXml(test2);

    std::cout<<"\n-------------\n";
    std::cout<<"-- YAML 1: --\n";
    std::cout<<"-------------\n";
    std::cout<< yaml_generator.GenerateYaml(test);
    std::cout<<"\n-------------\n";
    std::cout<<"-- YAML 2: --\n";
    std::cout<<"-------------\n";
    std::cout<< yaml_generator.GenerateYaml(test2);

    return 0;
}
