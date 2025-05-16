# MotionSentry
#### Copyright Radu Marin 2025

## Table of Contents
- [Overview](#overview)
- [Hardware Requirements](#hardware-requirements)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Installation](#installation)
- [Configuration](#configuration)
- [Web Interface](#web-interface)
- [Troubleshooting](#troubleshooting)
- [Pin Configuration](#pin-configuration)
- [Contributing](#contributing)

## Overview
MotionSentry is an intelligent security system that detects motion in its environment and tracks the source using a dual-servo mechanism. When motion is detected, the system alerts the user through visual tracking, audible alerts, and real-time notifications via a web interface. This project combines hardware sensors, servo motors, and WiFi connectivity to create a versatile security monitoring solution.

## Hardware Requirements
- ESP32 development board
- PIR motion sensor
- 2× Servo motors (Scanner & Sentry)
- Passive buzzer
- Jumper wires
- 5V power supply (2x battery pack + regulator)

## Features
- **Motion Detection**: Uses a PIR sensor with debounce protection to reliably detect movement
- **Dual-Servo Tracking System**:
  - Scanner servo: Continuously scans the environment for motion
  - Sentry servo: Moves to track detected motion sources
- **Audible Alerts**: Buzzer provides sound alerts when motion is detected
- **Real-time Web Interface**: Displays motion events with timestamps
- **Time Synchronization**: Uses NTP to maintain accurate event timing
- **FreeRTOS Task Management**: Separate tasks for scanning, motion tracking, and WiFi communications

## System Architecture

### Components
1. **PIR Sensor Module**
   - Detects infrared radiation changes caused by movement
   - Hardware debounce implementation prevents false triggers
   - Interrupt-driven for immediate response to motion events

2. **Servo Control System**
   - Scanner servo sweeps across detection area at regular intervals
   - Sentry servo moves to point at detected motion source
   - Coordinate transformation ensures accurate tracking

3. **Alert System**
   - PWM-driven buzzer provides configurable audio alerts
   - Web interface shows timestamp of latest detection

4. **WiFi Connectivity**
   - Hosts a web server for remote monitoring
   - Provides real-time updates of detection events
   - NTP time synchronization for accurate timestamps

### Software Architecture
The codebase is organized into several modules:

- **main.cpp**: System initialization and main control loop
- **pir.cpp**: PIR sensor functionality with debouncing
- **scanner.cpp**: Scanner servo control and position management
- **sentry.cpp**: Sentry servo control and motion tracking
- **tonePWM.cpp**: Buzzer control using ESP32's PWM capabilities
- **wifi.cpp**: WiFi connectivity and web server functionality
- **util.h**: Common utilities, constants, and helper functions

The system uses FreeRTOS for task management, with separate tasks for:
- Servo scanning operations
- WiFi connectivity and web server management
- Main loop for motion detection and tracking

## Installation

### Prerequisites
- PlatformIO IDE (recommended), Visual Studio Code + PlatformIO extension or Arduino IDE
- ESP32 board support package

### Build and Upload
1. Clone the repository
2. Open the project in PlatformIO, Vscode or Arduino IDE
3. Install required libraries:
   - ESP32Servo
   - WiFi
   - WebServer
4. Configure WiFi credentials in `wifi.cpp`
5. Build and upload to your ESP32

## Configuration
Key parameters can be adjusted in the utility header files:

### Motion Detection Settings
- `PIR_PIN`: GPIO pin for PIR sensor
- `CALIBRATION_TIME`: Initial PIR calibration period
- `DEBOUNCE_TIME`: Debounce time for motion detection

### Servo Settings
- `SENTRY_PIN`: GPIO pin for sentry servo
- `SCANNER_PIN`: GPIO pin for scanner servo
- `SERVO_FREQ`: PWM frequency for servo control
- `SERVO_MIN_PW`/`SERVO_MAX_PW`: Servo pulse width limits
- `MAX_ANGLE`: Maximum servo rotation angle
- `SWEEP_TIME`: Time to complete a sweep of the area

### Alert Settings
- `BUZZER_PIN`: GPIO pin for buzzer
- `BUZZER_FREQ`: Alert tone frequency
- `ALERT_TIME`: Duration of alert sound
- `WAIT_TIME`: Delay between alerts

### WiFi Settings
- `CONNECT_TIMEOUT`: WiFi connection timeout
- `CONNECT_RETRIES`: Number of connection attempts

## Web Interface
The device hosts a web interface accessible via its IP address, which displays:

- Current device status
- Timestamp of the last motion detection event
- Auto-refreshing data (every 2 seconds)

The interface is responsive and works on mobile and desktop browsers.

## Troubleshooting

### Common Issues
1. **PIR sensor triggering too frequently**
   - Increase `DEBOUNCE_TIME` in the configuration
   - Check for sources of interference like air vents or heat sources

2. **Servos not tracking accurately**
   - Verify servo calibration values
   - Adjust the angle transformation function in `sentry.cpp`

3. **WiFi connectivity issues**
   - Check SSID and password in `wifi.cpp`
   - Verify ESP32 is within WiFi range
   - Increase `CONNECT_RETRIES` if needed

4. **Time synchronization failure**
   - Ensure internet connectivity
   - Try alternative NTP servers

## Pin Configuration

| Component     | Default Pin | Description                    |
|---------------|-------------|--------------------------------|
| PIR Sensor    | See util.h  | Motion detection input         |
| Scanner Servo | See util.h  | Area scanning servo control    |
| Sentry Servo  | See util.h  | Motion tracking servo control  |
| Buzzer        | See util.h  | Alert sound output             |

## Contributing
Contributions to the project are welcome. Please follow these steps:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

Please ensure your code follows the existing style and includes appropriate comments.
