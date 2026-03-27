//
// Created by Piotr Gdański on 13/02/2026.
//

#ifndef OBD_READER_UTILS_H
#define OBD_READER_UTILS_H

#pragma once
#include <string>

enum exit_code {
    SUCCESS=0,
    ERROR_DETECTION=1,
    ERROR_CONNECTION=2,
};

struct connection_data {
    std::string port_name{};
    int baud_rate {-1};
    bool success {false};
};

connection_data detect_port();


#endif //OBD_READER_UTILS_H