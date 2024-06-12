This is the irrigation scheduler currently under development.

It consists of..
- one (and possibly later multiple) PCB designs in KiCad
- An Arduino Program for ESP32

It depends on

- ArduPar3
- ArduParWeb

When finished, it will provide the following functionality:

- Actuating solenoid valves, fans, lights and other actuators based on schedules and/or sensor measurements
- Web interface for configuring schedules and other settings
- logging and display of sensor data
- external RTC support for hassle free time keeping

