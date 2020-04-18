//
// Created by filip on 18.04.2020.
//

#include "Element.h"

void Element::setValue(std::string value, bool isAttribute = false) {
    this->value = std::move(value);
    this->resetChildren();
    this->hasValue = true;
    this->attribute = isAttribute;

}


bool Element::addChild(std::shared_ptr<Element> child) {
    this->children.emplace_back(std::move(child));
    if(this->hasValue)
    {
        this->value = "";
        this->hasValue = false;
    }

}

Element::Element(const std::string &name, const std::string &value, bool isAttribute) :
name(name), value(value), hasValue(true), attribute(isAttribute) {}

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

bool Element::isAttribute() const {
    return attribute;
}

void Element::setIsAttribute(bool isAttribute) {
    Element::attribute = isAttribute;
}


