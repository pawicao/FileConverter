//
// Created by filja on 12.06.2020.
//

#include <numeric>
#include <algorithm>
#include "XMLGenerator.h"
namespace XML
{
    std::string XMLGenerator::GenerateXml(const Document &document)
    {
        std::string xml;
        std::for_each(document.getElements().begin(), document.getElements().end(),
                [&](const Element& e) {xml += Generate(e);});
        return xml;
    }
    std::string XMLGenerator::Generate(const Element &element)
    {
        return printOpeningTag() + printName(element) + printAttributes(element) + printClosingTag(element)
        + printValue(element) + printChildren(element) + printEndingTag(element);
    }

    const std::string &XMLGenerator::printName(const Element &element) const
    { return element.getName(); }

    std::string XMLGenerator::printEndingTag(const Element &element)
    {
        if (element.getChildren().empty() && !element.hasValueFun())
        {
            return "";
        }
        if(element.hasValueFun())
        {
            return "</" + element.getName() + ">\n";
        }
        return printWithProperTabulation("</" + element.getName() + ">\n");
    }

    std::string XMLGenerator::printChildren(const Element &element)
    {
        if (element.getChildren().empty())
        {
            return "";
        }
        tabCount++;
        std::string tmp;
        std::for_each(element.getChildren().begin(), element.getChildren().end(),
                      [&](const std::shared_ptr<Element> &child)
                      { tmp += Generate(*child); });
        tabCount--;
        return tmp;
    }

    std::string XMLGenerator::printValue(const Element &element)
    {
        return element.hasValueFun() ? element.getValue() : "";
    }

    std::string XMLGenerator::printClosingTag(const Element &element)
    {
        if (element.getChildren().empty() && !element.hasValueFun())
        {
            return "/>\n";
        }
        if (element.hasValueFun())
        {
            return ">";
        }
        return ">\n";
    }

    std::string XMLGenerator::printAttributes(const Element &element)
    {
        if(element.getAttributes().empty())
        {
            return "";
        }
        std::string s;
        std::for_each(element.getAttributes().begin(), element.getAttributes().end(),
                      [&](const std::pair<std::string, std::string> &attribute)
                      { s += " " +attribute.first + "=\"" + attribute.second + "\""; });
        return s;
    }

    std::string XMLGenerator::printOpeningTag()
    { return printWithProperTabulation("<"); }

    std::string XMLGenerator::printWithProperTabulation(const std::string &printable)
    {
        return std::string(tabCount, '\t') + printable;
    }

    XMLGenerator::XMLGenerator() : tabCount(0){}
}