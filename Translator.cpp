//
// Created by Piotr Gdański on 11/01/2026.
//
#include <iostream>
#include <string>
#include "Translator.h"
#include "SerialPort.h"

// TODO: Make clean functions ready for other formats of data (extra whitespace etc.)
std::string one_number_clean(const std::string& result, const std::string& expected_return) {
    if (result[result.size()-1] != '>') {
        return "-1";
    }

    const std::string cut_result {result.substr(0, result.size()-3)};

    const std::size_t header_pos = cut_result.find(expected_return);

    if (header_pos == std::string::npos) {
        return "-1";
    }

    return cut_result.substr(header_pos + 6);

}


std::pair<std::string, std::string> two_number_clean(const std::string& result, const std::string& expected_return) {
    if (result[result.size()-1] != '>') {
        return std::make_pair("-1", "-1");
    }

    const std::string cut_result {result.substr(0, result.size()-3)};

    const std::size_t header_pos = cut_result.find(expected_return);

    if (header_pos == std::string::npos) {
        return std::make_pair("-1", "-1");
    }

    const std::string num1 {cut_result.substr(header_pos + 6, 2)};
    const std::string num2 {cut_result.substr(header_pos + 9, 2)};

    return std::make_pair(num1, num2);;
}


int get_speed(const serial_port& port){
    const std::string message {"01 0D\r"};

    if (port.write_to_port(message) == -1) {
        // Error writing to port
        return -1;
    }

    const std::string result {port.read_until_timeout('>')};

    const std::string clean_result {one_number_clean(result, "41 0D")};

    const int speed { std::stoi(clean_result, nullptr, 16)};
    return speed;
}

int get_rpm(const serial_port& port) {
    const std::string message {"01 0C\r"};

    if (port.write_to_port(message) == -1) {
        return -1;
    }

    const std::string result {port.read_until_timeout('>')};

    const std::pair<std::string, std::string> clean_result {two_number_clean(result, "41 0C")};

    const int num1 { std::stoi(clean_result.first, nullptr, 16)};
    const int num2 { std::stoi(clean_result.second, nullptr, 16)};
    return (256*num1+num2)/4;
}

int get_engine_load(const serial_port& port) {
    const std::string message {"01 04\r"};
    if (port.write_to_port(message) == -1) {
        return -1;
    }
    const std::string result {port.read_until_timeout('>')};
    std::string clean_result {one_number_clean(result, "41 04")};

    const int a { std::stoi(clean_result, nullptr, 16)};
    return (a*100)/255;
}

int get_coolant_temp(const serial_port& port) {
    const std::string message {"01 05\r"};
    if (port.write_to_port(message) == -1) {
        return -1;
    }
    const std::string result {port.read_until_timeout('>')};
    const std::string clean_result {one_number_clean(result, "41 05")};

    const int a { std::stoi(clean_result, nullptr, 16)};
    return a - 40;
}

int get_intake_temp(const serial_port& port) {
    const std::string message {"01 0F\r"};
    if (port.write_to_port(message) == -1) {
        return -1;
    }
    const std::string result {port.read_until_timeout('>')};
    const std::string clean_result {one_number_clean(result, "41 0F")};

    const int a { std::stoi(clean_result, nullptr, 16)};
    return a - 40;
}

int get_throttle_position(const serial_port& port) {
    const std::string message {"01 11\r"};
    if (port.write_to_port(message) == -1) {
        return -1;
    }
    const std::string result {port.read_until_timeout('>')};
    const std::string clean_result {one_number_clean(result, "41 11")};

    const int a { std::stoi(clean_result, nullptr, 16)};
    return (a * 100) / 255;
}