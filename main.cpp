#include <iostream>
#include <memory>
#include "document/Document.h"
#include "XML/XMLLexer.h"
#include "XML/XMLParser.h"
int main() {
    std::shared_ptr<XML::XMLLexer> lexer(new XML::XMLLexer("output-onlinerandomtools.txt"));
    XML::XMLParser parser(lexer);

    Document test = parser.parse();


    return 0;
}
