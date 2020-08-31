cmake_minimum_required(VERSION 3.16)
project(SerialUploader)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(src SOURCES)

add_executable(SerialUploader ${SOURCES} src/main.cpp)