cmake_minimum_required(VERSION 3.6)
project(InformationTheoryMidterm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp HuffmanTree.cpp HuffmanTree.h Node.cpp Node.h)
add_executable(InformationTheoryMidterm ${SOURCE_FILES})