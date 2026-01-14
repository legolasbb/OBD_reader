//
// Created by Piotr Gdański on 11/01/2026.
//

#ifndef OBD_READER_TRANSLATOR_H
#define OBD_READER_TRANSLATOR_H

#endif //OBD_READER_TRANSLATOR_H

#include "SerialPort.h"
//TODO: get supported PID's

///@param port Reference to the active OBD port
///@return Vehicle's speed in km/h
int get_speed(const serial_port& port);

///@param port Reference to the active OBD port
///@return Vehicle's engine RPM (rounded down to nearest whole number)
int get_rpm(const serial_port& port);

///@param port Reference to the active OBD port
///@return Vehicle's engine load in %
int get_engine_load(const serial_port& port);

///@param port Reference to the active OBD port
///@return Vehicle's engine coolant temperature in Celsius degrees
int get_coolant_temp(const serial_port& port);

///@param port Reference to the active OBD port
///@return Vehicle's intake temperature in Celsius degrees
int get_intake_temp(const serial_port& port);

///@param port Reference to the active OBD port
///@return Vehicles throttle position in %
int get_throttle_position(const serial_port& port);