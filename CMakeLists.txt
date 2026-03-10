cmake_minimum_required(VERSION 4.1)
project(OBD_reader)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(OBD_reader
        SerialPort.cpp
        SerialPort.h
        Translator.h
        Translator.cpp
        utils.cpp
        utils.h)
add_executable(run_tests
        SerialPort.cpp
        SerialPort.h
        Translator.h
        Translator.cpp
        utils.cpp
        utils.h
        translator_tests.cpp)
