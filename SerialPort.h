//
// Created by Piotr Gdański on 07/01/2026.
//

#ifndef OBD_READER_SERIALPORT_H
#define OBD_READER_SERIALPORT_H

#include <iostream>
#include <string>

/// @class serial_port
/// @brief A hardware abstraction layer managing the lifecycle and configuration of the serial connection.
///
/// @details
/// This class encapsulates low-level OS primitives to facilitate communication with external OBD-II interfaces.
/// It is responsible for configuring connection parameters (baud rate, parity), managing data buffers,
/// and handling timeouts to ensure robust, synchronous data exchange with the vehicle's ECU.

class serial_port
{
    public:
        serial_port();

        ~serial_port();

        /// @brief Opens and configures the serial port connection.
        /// @param port_path The system path to the serial device (e.g., "/dev/ttyUSB0" or "/dev/ttyS0").
        /// @param baudRate The communication speed (e.g., 9600, 38400, 115200).
        /// @return True if the port was successfully opened and configured; false otherwise.
        bool open_port(const std::string& port_path, int baudRate);

        /// @brief Closes the serial port connection.
        void close_port();

        // I/O
        /// @brief Sends a string of data to the connected serial device.
        /// @param message The raw string command to send.
        /// @return The number of bytes successfully written, or -1 if an error occurred.
        int write_to_port(const std::string& message) const;

        /// @brief Reads characters from the port until a specific terminator character is encountered.
        /// @details This function blocks execution until the terminator is found, the device disconnects,
        /// or the internal read timeout (configured in termios) expires.
        /// @param terminator The character that signals the end of the message (e.g., '>').
        /// @return The accumulated string excluding the terminator, or partial data on error.
        std::string read_until_timeout(char terminator = '>') const;
    private:
        int file_descriptor;

        /// @brief Configures the low-level termios structure for the open file descriptor.
        /// @details Sets the port to Raw Mode, 8N1 (8 data bits, No parity, 1 stop bit), disables flow control,
        /// and sets read timeouts (VTIME).
        /// @param baudRate The requested baud rate to set.
        /// @return True if attributes were set successfully; false otherwise.
        bool configure_port(int baudRate) const;

};


#endif //OBD_READER_SERIALPORT_H