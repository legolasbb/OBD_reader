# OBD Reader

A C++17 prototype for reading live vehicle data through an ELM327-compatible OBD-II adapter over a serial connection.

OBD Reader scans for an adapter, requests OBD-II data, and translates hexadecimal responses into readable values. The current application displays vehicle speed and engine RPM in a terminal dashboard. A Python simulator lets you try it without a vehicle or physical adapter.

**Status:** work in progress. The terminal dashboard and serial backend are implemented; device filtering, a graphical interface, and Bluetooth connectivity are planned.

## Current features

- Automatic adapter detection by scanning `/dev`, trying several baud rates, and checking the response to an ELM327 reset command.
- Serial communication using POSIX APIs, with port configuration and read timeouts.
- A live terminal dashboard that polls speed and RPM, with a 100 ms pause between polling cycles.
- Translation functions for six OBD-II parameters:

| Parameter | Mode / PID | Unit | Displayed in dashboard |
| --- | --- | --- | --- |
| Vehicle speed | `01 0D` | km/h | Yes |
| Engine RPM | `01 0C` | RPM | Yes |
| Engine load | `01 04` | % | No |
| Coolant temperature | `01 05` | °C | No |
| Intake air temperature | `01 0F` | °C | No |
| Throttle position | `01 11` | % | No |

The terminal dashboard and connection messages are in English, with **Speed** shown in km/h and **RPM** shown in revolutions per minute. Available parameters depend on the vehicle and adapter.

## Requirements

- A POSIX environment with serial-port support. The current implementation targets macOS / Unix-like systems and has no native Windows backend.
- A C++17-capable compiler.
- **CMake 4.1 or newer**, as required by `CMakeLists.txt`.
- An ANSI-compatible terminal for the dashboard.
- Either an ELM327-compatible adapter exposed as a serial device, with permission to access it, or the included simulator.
- Python 3 for the simulator; it uses only the standard library and requires POSIX pseudo-terminal support.

## Build

From the repository root:

```sh
cmake -S . -B build
cmake --build build
```

This builds two executables: `OBD_reader` (the dashboard) and `run_tests` (the translator checks).

## Run with a vehicle

1. Connect the adapter to the vehicle's OBD-II socket and expose its serial connection to your computer.
2. Switch on the vehicle's ignition so the adapter and ECU can respond.
3. Run the application from the repository root:

   ```sh
   ./build/OBD_reader
   ```

The application automatically searches for the adapter and opens the detected port. There are currently no command-line options for selecting a port or baud rate. Once connected, the dashboard updates continuously; press **Ctrl+C** to stop it.

## Run with the simulator

In one terminal, start the simulator and leave it running:

```sh
python3 tools/elm_sim.py
```

Choose one of the prompted modes:

- **`0` — Simple test mode:** fixed responses for all six parameters, used by `run_tests`.
- **`1` — Real life data simulator:** changing values for trying the live dashboard. This is an approximate demo, not a validated vehicle model.

The simulator prints its pseudo-terminal path. In a second terminal, run:

```sh
./build/OBD_reader
```

The reader must be able to discover that pseudo-terminal. It currently scans only immediate entries in `/dev`; on Linux, simulator ports usually live under `/dev/pts/` and are therefore not discovered. The current simulator workflow is best suited to macOS.

Press **Ctrl+C** in each terminal to stop the dashboard and simulator.

## Translator checks

Start the simulator in **mode `0`**, then run this command in another terminal:

```sh
./build/run_tests
```

The checks expect speed **140 km/h**, RPM **775**, coolant temperature **90 °C**, intake temperature **25 °C**, engine load **50%**, and throttle position **20%**. Inspect the output for six `passed` messages.

These checks require a discoverable simulator port and are not registered with CTest. A value mismatch prints `failed` but does not currently produce a nonzero exit code.

## Roadmap

- [ ] **Device filtering:** narrow discovery to relevant serial devices and OBD adapters to reduce startup time and unnecessary probes.
- [ ] **Graphical user interface:** add a visual dashboard for live telemetry and connection status.
- [ ] **Bluetooth connection:** support wireless OBD-II adapters with connection setup and management.

## Project layout

```text
backend/
  main.cpp              Adapter connection and terminal dashboard
  SerialPort.cpp/.h     Serial-port configuration and I/O
  Translator.cpp/.h     OBD-II requests and value conversion
  utils.cpp/.h          Port discovery and connection metadata
  translator_tests.cpp  Simulator-based translator checks
tools/
  elm_sim.py            ELM327 serial simulator
CMakeLists.txt          Application and check targets
```

## License

Licensed under the [MIT License](LICENSE).
