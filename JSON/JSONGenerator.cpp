//
// Created by joanna on 14.06.2020.
//
#include <algorithm>
#include "JSONGenerator.h"

namespace JSON{

    std::string JSONGenerator::GenerateJSON(const Document &document)
    {
        std::string json;
        std::for_each(document.getElements().begin(), document.getElements().end(),
                      [&](const Element& e) {json += Generate(e);});
        return json;
    }

    std::string JSONGenerator::Generate(const Element &element){
        return printOpeningTag(element.getType()) + printAttributes(element)
        + printKey(element) + printValue(element) + printChildren(element)
        + printClosingTag(element.getType()) + ", ";
    }

    std::string JSONGenerator::printChildren(const Element &element){
        if (element.getChildren().empty()){
            return "";
        }
        tabCount++;
        std::string tmp;
        std::for_each(element.getChildren().begin(), element.getChildren().end(),
                      [&](const std::shared_ptr<Element> &child)
                      { tmp += Generate(*child); });
        tmp.pop_back();
        tabCount--;
        return tmp;
    }

    std::string JSONGenerator::printOpeningTag(int type){
        switch (type) {
            case 0:
                return "";
            case 1:
                return printWithProperTabulation("[");
            case 2:
                return printWithProperTabulation("{");
        }
        return "";
    }

    std::string JSONGenerator::printClosingTag(int type){
        switch (type) {
            case 0:
                return "";
            case 1:
                return printWithProperTabulation("]");
            case 2:
                return printWithProperTabulation("}");
        }
        return "";
    }

    std::string JSONGenerator::printWithProperTabulation(const std::string &printable){
        return std::string(tabCount, '\t') + printable;
    }

    std::string JSONGenerator::printKey(const Element &element){
        return element.getName() == "item" ? "" : "\"" + element.getName() + "\" : ";
    }

    std::string JSONGenerator::printValue(const Element &element){
        return element.hasValueFun() ? element.getValue() + ", " : "";
    }

    std::string JSONGenerator::printAttributes(const Element &element){
        if(element.getAttributes().empty()){
            return "";
        }
        std::string s;
        s += printOpeningTag(1);
        std::for_each(element.getAttributes().begin(), element.getAttributes().end(),
                      [&](const std::pair<std::string, std::string> &attribute)
                      { s += " " +attribute.first + " : " + attribute.second + ","; });
        s.pop_back();
        s += printClosingTag(1);
        return s;
    }

    JSONGenerator::JSONGenerator() : tabCount(0){}

}
