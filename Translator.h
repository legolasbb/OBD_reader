//
// Created by Piotr Gdański on 11/01/2026.
//

#ifndef OBD_READER_TRANSLATOR_H
#define OBD_READER_TRANSLATOR_H

#endif //OBD_READER_TRANSLATOR_H

#include "SerialPort.h"
//TODO: get supported PID's


int get_speed();

int get_rpm();

int get_engine_load();

int get_coolant_temp();

int get_intake_temp();

int get_throttle_position();