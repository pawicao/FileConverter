//
// Created by Oskar on 13.06.2020.
//

#include <algorithm>
#include <numeric>
#include "YAMLGenerator.h"

namespace YAML {
    std::vector<std::string> YAMLGenerator::split(const std::string &text, char sep) {
        std::vector<std::string> tokens;
        std::size_t start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos) {
            if (end != start) {
                tokens.push_back(text.substr(start, end - start));
            }
            start = end + 1;
        }
        if (end != start) {
            tokens.push_back(text.substr(start));
        }
        return tokens;
    }

    std::string YAMLGenerator::GenerateYaml(const Document &document) {
        std::string yaml;
        std::vector<bool> printed(document.getElements().size(), false);
        std::vector<Element> array_indices;
        for(int i = 0; i < document.getElements().size(); ++i) {
            if(!printed[i]) {
                array_indices.clear();
                if(i != document.getElements().size()-1) {
                    for(int j = i + 1; j < document.getElements().size(); ++j) {
                        if(document.getElements()[j].getName() == document.getElements()[i].getName()) {
                            if(array_indices.empty()) array_indices.push_back(document.getElements()[i]);
                            array_indices.push_back(document.getElements()[j]);
                            printed[j] = true;
                        }
                    }
                }
                if(array_indices.empty() && !printed[i]) {
                    yaml += Generate(document.getElements()[i], 0, false);
                }
                else {
                    yaml += GenerateArray(array_indices, 0);
                }
            }
        }
        return yaml;
    }

    std::string YAMLGenerator::GenerateStructure(const std::vector<std::shared_ptr<Element>> & elements,
            const int & depth_level) {
        std::string result;
        std::vector<bool> printed(elements.size(), false);
        std::vector<Element> array_indices;
        for(int i = 0; i < elements.size(); ++i) {
            if(!printed[i]) {
                array_indices.clear();
                if(i != elements.size()-1) {
                    for(int j = i + 1; j < elements.size(); ++j) {
                        if(elements[j]->getName() == elements[i]->getName()) {
                            if(array_indices.empty()) array_indices.push_back(*elements[i]);
                            array_indices.push_back(*elements[j]);
                            printed[j] = true;
                        }
                    }
                }
                if(array_indices.empty() && !printed[i]) {
                    result += Generate(*elements[i], depth_level, false);
                }
                else {
                    result += GenerateArray(array_indices, depth_level);
                }
            }
        }
        return result;
    }

    std::string YAMLGenerator::GenerateArray(const std::vector<Element> & elements, const int & depth_level) {
        std::string result;
        for(int i = 0; i < depth_level; ++i) {
            result += ' ';
        }
        result += printName(elements[0]) + ":\n";
        for(const auto & element : elements) {
            result += Generate(element, depth_level+1, true);
        }
        return result;
    }

    std::string YAMLGenerator::Generate(const Element &element, const int & depth_level, bool array_mode) {
        std::string result;
        int extra_indent, extra_indent_block;
        for(int i = 0; i < depth_level; ++i) {
            result += ' ';
        }
        if(array_mode) {
            result += "- ";
            extra_indent = 1;
            extra_indent_block = 3;
        }
        else {
            result += printName(element) + ":";
            extra_indent = 2;
            extra_indent_block = 2;
        }
        if(!element.getAttributes().empty()) {
            result += '\n';
            for(const auto & attribute : element.getAttributes()) {
                result += printAttribute(attribute, depth_level + extra_indent);
            }
            if(element.hasValueFun()) {
                for(int i = 0; i < depth_level +extra_indent; ++i) {
                    result += ' ';
                }
                result += "__text: " + printValue(element, depth_level + extra_indent_block);
            }
        }
        else if(element.hasValueFun()) {
            result += ' ' + printValue(element, depth_level + extra_indent_block);
        }
        result += result.back() != '\n' ? "\n" : "";
        if(!element.hasValueFun() && !element.getChildren().empty()) {
            result += GenerateStructure(element.getChildren(), depth_level + extra_indent);
        }
        return result;
    }

    std::string YAMLGenerator::printAttribute(const std::pair<std::string, std::string> & attribute,
            const int & depth_level) {
        std::string result;
        for (int i = 0; i < depth_level; ++i) {
            result += ' ';
        }
        return result + "_" + attribute.first + ": " + attribute.second + '\n';
    }

    const std::string YAMLGenerator::printName(const Element &element) const {
        return element.getName().empty() ? "root" : element.getName();
    }

    std::string YAMLGenerator::printValue(const Element &element, const int & depth_level) {
        if(element.getValue().find('\n') == std::string::npos) {
            return element.getValue() + '\n';
        }
        std::string value = element.getValue();
        std::string result;
        result = "|";
        if(value.back() == '\n') {
            value.pop_back();
        }
        else {
            result += '-';
        }
        result += '\n';
        std::vector <std::string> values_vector = split(value, '\n');
        for(const auto & i : values_vector) {
            for(int j = 0; j < depth_level; ++j) {
                result += ' ';
            }
            result += i + "\n";
        }
        return result;
    }

    YAMLGenerator::YAMLGenerator() = default;
}