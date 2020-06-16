#include <iostream>
#include <memory>
#include <algorithm>
#include <cstring>
#include "document/Document.h"
#include "XML/XMLLexer.h"
#include "JSON/JSONLexer.h"
#include "JSON/JSONParser.h"
//#include "JSON/JSONGenerator.h"
#include "XML/XMLParser.h"
#include "YAML/YAMLLexer.h"
#include "YAML/YAMLParser.h"
#include "XML/XMLGenerator.h"
#include "YAML/YAMLGenerator.h"
#include "JSON/JSONGenerator.h"

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char * argv[]) {

    if(cmdOptionExists(argv, argv+argc, "--help")) {
        std::cout<<"File Converter - made by Filip Jachura, Joanna Kusek and Oskar Pawica"<<std::endl;
        std::cout<<"XML/JSON/YAML -> Abstract Syntax Tree -> XML/JSON/YAML"<<std::endl<<std::endl;
        std::cout<<"Usage: FileConverter [options...]\n";
        std::cout<<" -i <PATH>\t\tPath of the file to be parsed.\n";
        std::cout<<" -o <PATH>\t\tPath of the file to be written to. If not specified, stdout is used.\n";
        std::cout<<" -l <xml/yaml/json>\tResult language (can be xml, yaml or json).\n";
        return 0;
    }

    if(!cmdOptionExists(argv, argv+argc, "-i")) {
        std::cerr<<"Error: You must declare the input file with -i option."<<std::endl;
        std::cerr<<"See --help for the list of options"<<std::endl;
        return -1;
    }

    if(!cmdOptionExists(argv, argv+argc, "-l")) {
        std::cerr<<"Error: You must declare the result language with the -l option."<<std::endl;
        std::cerr<<"See --help for the list of options"<<std::endl;
        return -1;
    }

    char * input_file = getCmdOption(argv, argv + argc, "-i");

    if (!input_file) {
        std::cerr<<"Error: No input file specified"<<std::endl;
        std::cerr<<"See --help for the list of options"<<std::endl;
        return -1;
    }

    std::string file(input_file);
    Document document;

    if(file.length() > 3 && file.substr(file.length() - 4) == ".xml") {
        std::shared_ptr<XML::XMLLexer> xml_lexer(new XML::XMLLexer(file));
        XML::XMLParser xml_parser(xml_lexer);
        document = xml_parser.parse();
    }
    else if (file.length() > 4 && file.substr(file.length() - 5) == ".json"){
        std::shared_ptr<JSON::JSONLexer> json_lexer(new JSON::JSONLexer(file));
        JSON::JSONParser json_parser(json_lexer);
        document = json_parser.parse();
    }
    else if( (file.length() > 4 && file.substr(file.length() - 5) == ".yaml") ||
            (file.length() > 3 && file.substr(file.length() - 4) == ".yml") ) {
        std::shared_ptr<YAML::YAMLLexer> yaml_lexer(new YAML::YAMLLexer(file));
        YAML::YAMLParser yaml_parser(yaml_lexer);
        document = yaml_parser.parse();
    }
    else {
        std::cerr<<"Error: Wrong input file. Must be xml, yaml or json."<<std::endl;
        std::cerr<<"See --help for the list of options"<<std::endl;
        return -1;
    }

    char * result_language = getCmdOption(argv, argv + argc, "-l");

    if (!result_language) {
        std::cerr<<"Error: No result language specified. Must be xml, yaml or json."<<std::endl;
        std::cerr<<"See --help for the list of options"<<std::endl;
        return -1;
    }

    std::string result_string;

    if(strcmp(result_language, "xml") == 0) {
        XML::XMLGenerator xml_generator{};
        result_string = xml_generator.GenerateXml(document);
    }
    else if(strcmp(result_language, "json") == 0) {
        JSON::JSONGenerator json_generator{};
        result_string = json_generator.GenerateJSON(document);
    }
    else if(strcmp(result_language, "yml") == 0 || strcmp(result_language, "yaml") == 0) {
        YAML::YAMLGenerator yaml_generator{};
        result_string = yaml_generator.GenerateYaml(document);
    }
    else {
        std::cerr<<"Error: Wrong result language. Must be xml, yaml or json."<<std::endl;
        std::cerr<<"See --help for the list of options"<<std::endl;
        return -1;
    }

    if(cmdOptionExists(argv, argv+argc, "-o")) {
        char * output_file = getCmdOption(argv, argv + argc, "-o");
        if(!output_file) {
            std::cerr<<"Error: Wrong usage of output file option."<<std::endl;
            std::cerr<<"See --help for the list of options"<<std::endl;
        }
        std::ofstream ofs;
        ofs.open (output_file, std::ofstream::out | std::ofstream::trunc);
        ofs <<result_string;
        ofs.close();
    }
    else {
        std::cout<<result_string;
    }


    return 0;
}
