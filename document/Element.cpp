//
// Created by filip on 18.04.2020.
//

#include "Element.h"


void Element::addChild(std::shared_ptr<Element> child) {
    this->children.emplace_back(std::move(child));
    if(this->hasValue)
    {
        this->value = "";
        this->hasValue = false;
    }

}

Element::Element(const std::string &name, const std::string &value) :
name(name), value(value), hasValue(true) {}

Element::Element(const std::string &name) : name(name) {}

const std::string &Element::getName() const {
    return name;
}

void Element::setName(const std::string &name) {
    Element::name = name;
}

const std::string &Element::getValue() const {
    return value;
}

void Element::setValue(const std::string &value) {
    Element::setHasValue(true);
    Element::value = value;
}

const Children &Element::getChildren() const {
    return children;
}

void Element::setChildren(Children children) {
    Element::children = std::move(children);
}

bool Element::isHasValue() const {
    return hasValue;
}

void Element::setHasValue(bool hasValue) {
    Element::hasValue = hasValue;
}

void Element::addAttribute(const std::string &name, const std::string &value) {
    Element::attributes.emplace_back(std::make_pair(name,value));
}

const Attributes &Element::getAttributes() const {
    return attributes;
}


