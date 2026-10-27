# RFID & Fingerprint Attendance System

An ESP32-based attendance system developed using C++ and Arduino. The system combines RFID card scanning and fingerprint recognition to identify users and transmit attendance data to a PHP server over Wi-Fi.

## Overview

This project integrates RFID and biometric fingerprint technologies into an attendance system. Users can identify themselves using an RFID card or a registered fingerprint. The ESP32 communicates with a PHP backend to verify user information and submit attendance records.

An LCD displays system messages, while a buzzer and relay provide additional hardware feedback.

## Technologies Used

- **C++** — Main programming language
- **ESP32** — Main microcontroller
- **Arduino Framework** — Embedded software development
- **MFRC522** — RFID card reader
- **Fingerprint Sensor** — Biometric identification
- **LCD I2C 16×2** — Status and attendance messages
- **Wi-Fi & HTTP** — Communication with the backend server
- **PHP** — Backend endpoints for identity verification and attendance recording

## Key Features

- RFID card identification
- Fingerprint recognition
- Attendance verification through a PHP server
- Attendance data submission using HTTP requests
- LCD status and user feedback
- Buzzer notifications
- Relay activation after successful RFID verification

## System Workflow

1. The ESP32 connects to the configured Wi-Fi network.
2. The user scans an RFID card or places a registered finger on the fingerprint sensor.
3. The ESP32 sends the identification data to the PHP backend.
4. The backend verifies the identity and returns the associated user information.
5. Attendance data is submitted to the server for recording.
6. The LCD, buzzer, and relay provide feedback according to the program logic.

## Hardware Components

- ESP32 development board
- MFRC522 RFID reader
- RFID card or tag
- Fingerprint sensor compatible with the Adafruit Fingerprint library
- 16×2 I2C LCD
- Buzzer
- Relay module
- Jumper wires and supporting components

## Libraries

- `SPI.h`
- `WiFi.h`
- `MFRC522.h`
- `HTTPClient.h`
- `LiquidCrystal_I2C.h`
- `Adafruit_Fingerprint.h`

## Setup

1. Install the required libraries in the Arduino IDE.
2. Connect the hardware components to the ESP32 according to the pin configuration in the source code.
3. Configure the Wi-Fi credentials and backend server address.
4. Set up the PHP endpoints required for RFID and fingerprint verification and attendance recording.
5. Upload the `.ino` program to the ESP32.
6. Open the Serial Monitor to check device status and identification results.

## Project Purpose

This project demonstrates the application of C++ programming, embedded systems, RFID technology, biometric identification, and client-server communication in an attendance management system.

## Author

**David Dwi Ariyadi**

GitHub: [@haidapid](https://github.com/haidapid)

