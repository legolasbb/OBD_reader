//
// Created by Piotr Gdański on 07/01/2026.
//
// TODO: debuging and testing
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <termios.h>
#include <fcntl.h>

#include "SerialPort.h"

serial_port::serial_port() : file_descriptor(-1) {}

serial_port::~serial_port() {
    close_port();
}

bool serial_port::open_port(const std::string& port_path, int baudRate) {
    file_descriptor = open(port_path.c_str(), O_RDWR | O_NOCTTY);
    if (file_descriptor == -1) {
        std::cerr << "Error opening serial port " << strerror(errno) <<"\n";
        return false;
    }
    if (!serial_port::configure_port(baudRate)) {
        std::cerr << "Error setting up serial port " << "\n";
        close_port();
        return false;
    }
    return true;
}
void serial_port::close_port() {
    if (file_descriptor != -1) {
        close(file_descriptor);
        file_descriptor = -1;
    }
}

int serial_port::write_to_port(const std::string& message) const {
    if (file_descriptor == -1) {
        std::cerr << "Port is closed" << "\n";
    }

    ssize_t bytes_sent { write(file_descriptor, message.c_str(), message.length())};
    if (bytes_sent == -1) {
        std::cerr << "Error writing to serial port " << strerror(errno) <<"\n";
        return -1;
    }
    return static_cast<int>(bytes_sent);
}

std::string serial_port::read_until_timeout(char terminator) const{
    // TODO: add timeout
    std::string result {};
    char temp_buffer[1]{};
    while (true) {
            const ssize_t read_return {read(file_descriptor, temp_buffer, 1)};
        if (read_return == -1) {
            if (errno == EAGAIN) {
                continue;
            }
            std::cerr << "Read error " << strerror(errno) <<"\n";
            break;
        }
        else if (read_return == 0) {
            std::cerr << "Device disconnected" << "\n";
            break;
        }
        else {
            result+=temp_buffer[0];
            if (temp_buffer[0] == terminator) {
                //Full message read
                break;
            }
        }
    }
    return result;
}

bool serial_port::configure_port(const int baudRate) const {
    struct termios tty {};

    if (tcgetattr(file_descriptor, &tty) != 0) {
        std::cerr << "Error from tcgetattr\n";
        return false;
    }
    //setting speed to given baudrate
    speed_t speed {};
    switch (baudRate) {
        case 38400: speed = B38400; break;
        case 115200: speed = B115200; break;
        case 9600: speed = B38400; break;
        case 19200: speed = B19200; break;
        default:
            std::cerr << "Warning: Unsupported baud rate " << baudRate
                      << ". Defaulting to 38400.\n";
            speed = B38400;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    // Setting 8N1
    tty.c_cflag &= ~PARENB; // Disabling parity
    tty.c_cflag &= ~CSTOPB; // Setting one stop bit
    tty.c_cflag &= ~CSIZE; // Cleaning bit size setting
    tty.c_cflag |= CS8; // Setting bit size to 8

    // Disabling flow control
    tty.c_cflag &= ~CRTSCTS;

    //
    tty.c_cflag |= CREAD | CLOCAL;

    // Raw mode
    tty.c_lflag &= ~ICANON; // Turning cannonical mode off
    tty.c_lflag &= ~ECHO; // Turning echo off
    tty.c_lflag &= ~ECHOE; // Turing off erasure (backspace)
    tty.c_lflag &= ~ISIG; // Turning off signals

    //Turning off special data procesing
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off XON/XOFF
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // group of flags that change data from read
    tty.c_oflag &= ~OPOST; // Turing off input proccesing

    // Timeout when reading
    // Minimum number of bytes recived - 0
    // maximum wait time 100ms (
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1;

    // Saving settings
    if (tcsetattr(file_descriptor, TCSANOW, &tty) != 0) {
        std::cerr << "Error from tcsetattr\n";
        return false;
    }

    return true;
}
