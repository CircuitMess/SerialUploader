cmake_minimum_required(VERSION 3.16)
project(SerialUploader)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(src SOURCES)
list(APPEND SOURCES src/Serial.posix.impl src/Serial.windows.impl)

add_executable(SerialUploader ${SOURCES})