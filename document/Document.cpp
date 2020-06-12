//
// Created by filip on 19.04.2020.
//

#include "Document.h"

void Document::addNewElement(Element e) {
    this->elements.push_back(std::move(e));
}

void Document::setElements(const std::vector<Element> &elements)
{
    Document::elements = elements;
}

const std::vector<Element> &Document::getElements() const
{
    return elements;
}
