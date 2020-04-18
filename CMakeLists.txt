cmake_minimum_required(VERSION 3.10)
project(FileConverter)

set(CMAKE_CXX_STANDARD 17)

add_executable(FileConverter main.cpp document/Element.cpp document/Element.h document/Document.cpp document/Document.h)