//
// Created by filip on 18.04.2020.
//

#ifndef FILECONVERTER_ELEMENT_H
#define FILECONVERTER_ELEMENT_H


#include <string>
#include <variant>
#include <vector>
#include <memory>

class Element;

using Children = std::vector<std::shared_ptr<Element>>;
using Attributes = std::vector<std::pair<std::string, std::string>>;
using ElementPtr = std::shared_ptr<Element>;
class Element {
public:
    void addChild(std::shared_ptr<Element> child);


    Element() = default;
    Element(const std::string &name);

    Element(const std::string &name, const std::string &value);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::string &getValue() const;

    void setValue(const std::string &value);

    const Children &getChildren() const;

    void setChildren(Children children);

    bool hasValueFun() const;

    void setHasValue(bool hasValue);

    void addAttribute(const std::string& name, const std::string& value);

    const Attributes &getAttributes() const;

    int getType() const;

    void setType(int type);

private:
    std::string name;
    std::string value;
    Children children;
    bool hasValue = false; //if field has a value it cannot have children
    Attributes attributes; // <title lang="en"> - lang is attribute - must be handled different than normal field
    int type = 0; // 0-default ,1-array, 2-object

    inline void resetChildren()
    {
        children.clear();
    }

};


#endif //FILECONVERTER_ELEMENT_H
