//
// Created by Oskar on 13.06.2020.
//

#ifndef FILECONVERTER_YAMLGENERATOR_H
#define FILECONVERTER_YAMLGENERATOR_H


#include <string>
#include "../document/Document.h"

namespace YAML {
    class YAMLGenerator {

    public:
        YAMLGenerator();

        std::string GenerateYaml(const Document &document);

        std::string Generate(const Element &Element, const int &depth_level, bool array_mode);

    private:

        std::string printValue(const Element &element, const int &depth_level);

        const std::string &printName(const Element &element) const;

        std::string GenerateStructure(const std::vector<std::shared_ptr<Element>> &elements, const int &depth_level);

        std::string printAttribute(const std::pair<std::string, std::string> &attribute, const int &depth_level);

        std::string GenerateArray(const std::vector<Element> &elements, const int &depth_level);

        std::vector<std::string> split(const std::string &text, char sep);
    };
}


#endif //FILECONVERTER_YAMLGENERATOR_H
