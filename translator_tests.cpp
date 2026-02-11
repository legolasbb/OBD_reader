//
// Created by Piotr Gdański on 18/01/2026.
//
#include "string"

#include "Translator.h"

void assert_equal(int expected, int actual, std::string test_name) {
    if (expected == actual) {
        std::cout << "Test " << test_name << " passed" << "\n";
    }
    else {
        std::cout << "Test " << test_name << " failed" << "\n";
        std::cout << "Expected: "<< expected << " Actual:" << actual << "\n";
    }
}

void primary_test(const serial_port & test_port) {
    //Test constant, correlated with elm_sim
    constexpr int expected_speed {140};
    constexpr int expected_rpm = {775};
    constexpr int expected_coolant_temp {90};
    constexpr int expected_intake_temp {25};
    constexpr int expected_engine_load {50};
    constexpr int expected_throttle_position {20};

    const int speed {get_speed(test_port)};
    const int rpm {get_rpm(test_port)};
    const int coolant_temp {get_coolant_temp(test_port)};
    const int intake_temp {get_intake_temp(test_port)};
    const int engine_load {get_engine_load(test_port)};
    const int throttle_position {get_throttle_position(test_port)};

    assert_equal(expected_speed, speed, "speed");
    assert_equal(expected_rpm, rpm, "rpm");
    assert_equal(expected_coolant_temp, coolant_temp, "coolant_temp");
    assert_equal(expected_intake_temp, intake_temp, "intake_temp");
    assert_equal(expected_engine_load, engine_load, "engine_load");
    assert_equal(expected_throttle_position, throttle_position, "throttle_position");
}

int main() {
    std::string port_path {};
    std::cout<<"Insert port name: ";
    std::cin>>port_path;
    serial_port test_port{};
    test_port.open_port(port_path, 38400);

    primary_test(test_port);

    test_port.close_port();
    return 0;
}