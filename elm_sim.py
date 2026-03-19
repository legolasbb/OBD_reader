import pty
import os
import threading
import time
import math

TERMINATOR = b"\r\n>"

class Car_state:
    def __init__(self):
        self.rpm = 800
        self.speed = 100
        self.coolant_temp = 90
        self.throttle_position = 0
        self.intake_temp = 25

car = Car_state()

# Simulating real-life car parameters, changing based on sin function
def engine_sim():
    last_throttle_position = 0
    while True:
        car.throttle_position = abs(math.sin(time.time()))*100
        if last_throttle_position <= car.throttle_position:
            car.speed += 5
        else:
            car.speed -= 5
        last_throttle_position = car.throttle_position
        car.rpm = 800 + 50 * car.throttle_position
        car.coolant_temp += 0.01
        car.intake_temp += 0.01

        time.sleep(0.1)

threading.Thread(target=engine_sim, daemon=True).start()
def create_message(given_message, mode):
    return_response = b"41 0D %02X" % 140
    if given_message == b"01 0D":
        if mode == 0:
            #return speed
            #8C - 140 km/h
            return_response = b"41 0D %02X" % 140
        if mode == 1:
            cur_speed = int(car.speed)
            return_response = b"41 0D %02X" % cur_speed

    elif given_message == b"01 0C":
        if mode == 0:
            #return rpm
            # 775 - A = 12, B = 28
            return_response = b"41 0C %02X %02X" % (12, 28)
        else:
            cur_rpm = car.rpm
            a = cur_rpm // 256
            b = cur_rpm % 256
            return_response = b"41 0C %02X %02X" % (a, b)
    elif given_message == b"01 04":
        if mode == 0 or mode == 1:
            # return engine load
            # 128 dec = 80 hex -> 128 * 100 / 255 = 50%
            return_response = b"41 04 %02X" % 128
    elif given_message == b"01 05":
        if mode == 0:
            # return coolant temp (A - 40)
            # 130 dec = 82 hex -> 130 - 40 = 90
            return_response = b"41 05 %02X" % 130
        else:
            cur_coolant_temp = car.coolant_temp
            return_response = b"41 05 %02X" % cur_coolant_temp
    elif given_message == b"01 0F":
        if mode == 0:
            # return intake temp (A - 40)
            # 65 dec = 41 hex -> 65 - 40 = 25
            return_response = b"41 0F %02X" % 65
        else:
            cur_intake_temp = car.intake_temp
            return_response = b"41 0F %02X" % cur_intake_temp
    elif given_message == b"01 11":
        if mode == 0:
            # get throttle position
            # 51 dec = 33 hex -> 51 * 100 / 255 = 20%
            return_response = b"41 11 %02X" % 51
        else:
            cur_throttle_position = car.throttle_position
            return_response = b"41 11 %02X" % cur_throttle_position
    elif given_message == b"AT Z":
        print("Conncection try")
        return_response = b"ELM327 v1.5"
    return return_response

def simulation(port, mode):
    while True:
        buffer = os.read(port, 1)

        if not buffer:
            continue

        while True:
            new_data = os.read(port, 1)
            if new_data == b'\r':
                break
            buffer+=new_data
        read_message = buffer.strip()
        response = create_message(read_message, mode)
        full_packet = response+TERMINATOR
        os.write(port, full_packet)

def get_mode()->int:
    print("Choose work mode")
    print("0 - Simple test mode")
    print("1 - Real life data simulator")
    print("Type 0 or  1 to Choose")
    mode = int(input())
    if mode != 0 and mode != 1:
        print("Invalid mode")
        return get_mode()
    return mode

chosen_mode = get_mode()

master, slave = pty.openpty()
port_name = os.ttyname(slave)

simulation(master, chosen_mode)


