#include <iostream>
#include <thread>
#include <stdexcept>


#include "SerialPort.h"
#include "utils.h"
#include "Translator.h"

connection_data connection_attempt() {
    constexpr int attempts = 1;
    for (int i = 0; i < attempts; i++) {
        constexpr int wait_ms = 5000;
        connection_data detected_port{detect_port()};
        if (detected_port.success) {
            std::cout<<"Port detected successfully, proceeding\n";
            return detected_port;
        }
        std::cout<<"Port not detected successfully, trying again in"<< wait_ms/1000<<" seconds\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
    }

    throw std::runtime_error{"Port not detected successfully"};
}

void display_data(serial_port& car_port) {
    // Clear screen
    std::cout << "\033[2J\033[1;1H";

    while (true) {
        // Move cursor up
        std::cout << "\033[1;1H";

        int speed{get_speed(car_port)};
        int rpm{get_rpm(car_port)};

        std::cout << "===========================\n";
        std::cout << "    LIVE OBD DASHBOARD     \n";
        std::cout << "===========================\n";
        std::cout << "Predkosc: " << speed << " km/h    \n";
        std::cout << "Obroty:   " << rpm << " RPM      \n";
        std::cout << "===========================\n";

        std::cout << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    connection_data detected_port{};
    try {
        detected_port = connection_attempt();
    }
    catch (const std::runtime_error& e) {
        std::cout<<e.what()<<"\n";
        return exit_code::ERROR_DETECTION;
    }

    serial_port car_port;
    if (!car_port.open_port(detected_port.port_name, detected_port.baud_rate)) {
        std::cout<<"Unable to connect to port";
        return exit_code::ERROR_CONNECTION;
    }

    display_data(car_port);
}