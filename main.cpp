#include <iostream>
#include <memory>
#include "document/Document.h"
int main() {
    //Basic check to see if compiles and works
    Element element("testElement", "testValue");
    Element element1("testElement1");
    element1.setHasValue(false);
    element1.addAttribute("test", "test");
    element1.addChild(std::make_shared<Element>(element));
    element1.addChild(std::make_shared<Element>(element));
    Document doc;
    doc.addNewElement(element1);
    return 0;
}
