import pty
import os

def create_message(given_message):
    return_response = b"41 0D %02X" % 140
    if given_message == b"01 0D":
        #return speed
        #8C - 140 km/h
        return_response = b"41 0D %02X" % 140
    elif given_message == b"01 0C":
        #return rpm
        # 775 - A = 12, B = 28
        return_response = b"41 0C %02X %02X" % (12, 28)
    elif given_message == b"01 04":
        # return engine load
        # 128 dec = 80 hex -> 128 * 100 / 255 = 50%
        return_response = b"41 04 %02X" % 128
    elif given_message == b"01 05":
        # return coolant temp (A - 40)
        # 130 dec = 82 hex -> 130 - 40 = 90
        return_response = b"41 05 %02X" % 130
    elif given_message == b"01 0F":
        # return intake temp (A - 40)
        # 65 dec = 41 hex -> 65 - 40 = 25
        return_response = b"41 0F %02X" % 65
    elif given_message == b"01 11":
        # get throttle position
        # 51 dec = 33 hex -> 51 * 100 / 255 = 20%
        return_response = b"41 11 %02X" % 51
    return return_response


master, slave = pty.openpty()
port_name = os.ttyname(slave)

print(port_name)

TERMINATOR = b"\r\n>"

while True:
    buffer = os.read(master, 1)

    if not buffer:
        break

    while True:
        new_data = os.read(master, 1)
        if new_data == b'\r':
            break
        buffer+=new_data
    read_message = buffer.strip()
    response = create_message(read_message)
    full_packet = response+TERMINATOR
    os.write(master, full_packet)

