cmake_minimum_required(VERSION 4.1)
project(OBD_reader)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(OBD_reader
        backend/SerialPort.cpp
        backend/SerialPort.h
        backend/Translator.h
        backend/Translator.cpp
        backend/utils.cpp
        backend/utils.h
        backend/main.cpp)
add_executable(run_tests
        backend/SerialPort.cpp
        backend/SerialPort.h
        backend/Translator.h
        backend/Translator.cpp
        backend/utils.cpp
        backend/utils.h
        backend/translator_tests.cpp)
