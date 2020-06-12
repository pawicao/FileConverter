//
// Created by filip on 19.04.2020.
//

#ifndef FILECONVERTER_DOCUMENT_H
#define FILECONVERTER_DOCUMENT_H


#include "Element.h"

class Document {
public:
    void addNewElement(Element e);

    void setElements(const std::vector<Element> &elements);
    const std::vector<Element> &getElements() const;

private:
    std::vector<Element> elements;
};


#endif //FILECONVERTER_DOCUMENT_H
