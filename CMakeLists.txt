cmake_minimum_required(VERSION 4.1)
project(OBD_reader)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(OBD_reader
        SerialPort.cpp
        SerialPort.h
        Translator.h
        Translator.cpp)
add_executable(run_tests
        SerialPort.cpp
        SerialPort.h
        Translator.h
        Translator.cpp
        translator_tests.cpp)
