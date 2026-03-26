# OBD Reader
*Work in progress*

**OBD Reader** is a C++ application designed to interface directly with vehicle On-Board Diagnostics (OBD-II) systems. The primary goal of this project is to establish hardware communication, request real-time vehicle data (like RPM, speed, or engine temperature), and parse the incoming byte streams into readable formats.

## Current Progress

- [x] All port communication
- [x] Getting and translating most important data
- [x] Automatic port scanning and detection
- [x] First prototype displaying live data (in terminal)
- [ ] Graphical user interface