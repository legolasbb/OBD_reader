//
// Created by Piotr Gdański on 07/01/2026.
//

#ifndef OBD_READER_SERIALPORT_H
#define OBD_READER_SERIALPORT_H

#include <iostream>
#include <unistd.h>
#include <string>
#include <cerrno>
#include <cstring>
#include <termios.h>
#include <fcntl.h>


class serial_port
{
    public:
        serial_port();

        ~serial_port();

        // Opening port
        bool open_port(const std::string& port_path, int baudRate);

        void close_port();

        // I/O
        int write_to_port(const std::string& message) const;

        std::string read_until_timeout(char terminator = '>', int timeoutMs = 2000) const;
    private:
        int file_descriptor;
        bool configure_port(int baudRate) const;

};


#endif //OBD_READER_SERIALPORT_H