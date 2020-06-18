//
// Created by joanna on 14.06.2020.
//
#include <algorithm>
#include <regex>
#include "JSONGenerator.h"

namespace JSON{

    std::string JSONGenerator::GenerateJSON(const Document &document)
    {
        std::string json;
        json += "{\n";
        std::for_each(document.getElements().begin(), document.getElements().end(),
                      [&](const Element& e) {json += Generate(e);});
        json.pop_back();
        json.pop_back();
        json += "\n}";
        return json;
    }

    std::string JSONGenerator::Generate(const Element &element){
        std::regex number("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?");
        if(element.hasValueFun() && (std::regex_match(element.getValue(),number) || element.getValue()=="null" || element.getValue()=="true" || element.getValue()=="false")){
            return printAttributes(element) + printKey(element)
                + printValue(element)  + printChildren(element)
                + "," ;
        }else if(element.hasValueFun()){
            return printAttributes(element) + printKey(element)
            + "\"" + printValue(element) + "\""
            + printChildren(element) + ",";
        }

        return printAttributes(element) + printKey(element) + printOpeningTag(element.getType())
            + printValue(element) + printChildren(element)
            + "\n" + printClosingTag(element.getType());
    }

    std::string JSONGenerator::printChildren(const Element &element){
        if (element.getChildren().empty()){
            return "";
        }
        tabCount++;

        std::string tmp;
        std::for_each(element.getChildren().begin(), element.getChildren().end(),
                      [&](const std::shared_ptr<Element> &child){
                        tmp += printWithProperTabulation("");
                        tmp += Generate(*child);
                        tmp += "\n";
                      });
        tmp.pop_back();
        tmp.pop_back();
        tabCount--;
        return tmp;
    }

    std::string JSONGenerator::printOpeningTag(int type){
        switch (type) {
            case 0:
                return "";
            case 1:
                return "[\n" + printWithProperTabulationStart("");
            case 2:
                return "{\n" + printWithProperTabulationStart("");
        }
        return "";
    }

    std::string JSONGenerator::printClosingTag(int type){
        switch (type) {
            case 0:
                return ", ";
            case 1:
                return "], ";
            case 2:
                return printWithProperTabulationEnd("") + "}, ";
        }
        return "";
    }

    std::string JSONGenerator::printWithProperTabulation(const std::string &printable){
        return std::string(tabCount, '\t') + printable;
    }

    std::string JSONGenerator::printWithProperTabulationStart(const std::string &printable){
        return std::string(tabCount-1 >= 0 ? tabCount-1 : tabCount, '\t') + printable;
    }

    std::string JSONGenerator::printWithProperTabulationEnd(const std::string &printable){
        return std::string(tabCount+1 , '\t') + printable;
    }

    std::string JSONGenerator::printKey(const Element &element){
        return element.getName() == "item" ? "" : "\"" + element.getName() + "\" : ";
    }

    std::string JSONGenerator::printValue(const Element &element){
        return element.hasValueFun() ? element.getValue() : "";
    }

    std::string JSONGenerator::printAttributes(const Element &element){
        if(element.getAttributes().empty()){
            return "";
        }
        std::string s;
        s += "\"attributes\" : {";
        std::for_each(element.getAttributes().begin(), element.getAttributes().end(),
                      [&](const std::pair<std::string, std::string> &attribute)
                      { s += " \"" +attribute.first + "\" : " + attribute.second + ","; });
        s.pop_back();
        s += " }, \n";
        s += printWithProperTabulation("");
        return s;
    }

    JSONGenerator::JSONGenerator() : tabCount(0){}

}
