//
// Created by Piotr Gdański on 13/02/2026.
//

#include "utils.h"
#include "SerialPort.h"

#include <filesystem>
#include <iostream>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;

// Most popular baud rates eto test
constexpr int baud_rates[4] {38400, 115200, 9600, 19200};


std::vector<std::string> scan_ports() {
    std::vector<std::string> result;
    const fs::path ports_path("/dev");

    if (!fs::exists(ports_path) || !fs::is_directory(ports_path)) {
        std::cerr << "Div library does not exist";
        return result;
    }

    for (const auto& dev_path : fs::directory_iterator(ports_path)) {
        result.push_back(dev_path.path());
    }
    return result;
}

connection_data detect_port() {
    connection_data result;
    const std::vector<std::string> available_ports {scan_ports()};

    for (const auto& port_path : available_ports) {
        std::cerr << port_path;
        for (const int& baud_rate : baud_rates) {
            serial_port test_port;
            if (!test_port.open_port(port_path, baud_rate)) {
                continue;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Sending reset port command specific for ELM327
            int write_result = test_port.write_to_port("AT Z\r");
            if (write_result <= 0) {
                test_port.close_port();
                continue;
            }

            std::string response = test_port.read_until_timeout();
            std::cerr << response << std::endl;
            if (response.find("ELM") != std::string::npos) {
                // ELM adapter found and proper baud rate is set
                test_port.close_port();
                result.success = true;
                result.port_name = port_path;
                result.baud_rate = baud_rate;
                return result;
            }
            test_port.close_port();
        }
    }

    return result;
}

