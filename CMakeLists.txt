cmake_minimum_required(VERSION 3.10)
project(FileConverter)

set(CMAKE_CXX_STANDARD 17)

add_executable(FileConverter main.cpp document/Element.cpp document/Element.h document/Document.cpp document/Document.h
        XML/XML_tokens.h XML/XMLLexer.cpp XML/XMLLexer.h XML/XMLParser.cpp XML/XMLParser.h YAML/YAMLToken.h YAML/YAMLLexer.cpp YAML/YAMLLexer.h YAML/YAMLParser.cpp YAML/YAMLParser.h)