# 🎙️ Voice-Controlled Smart Home System

An AI-powered Smart Home System that integrates **Flutter**, **ESP32**, and **OpenAI Whisper** to enable **English and Arabic voice control** for home devices with real-time monitoring.

The system combines **Speech Recognition, Natural Language Processing, IoT, and Mobile Development** into a single intelligent home automation platform.

---

# 📑 Table of Contents

- Overview
- System Architecture
- Features
- Hardware Setup
- Backend Server
- Mobile Application
- Supported Commands
- Setup Instructions
- Critical Configuration
- Demo
- Future Improvements
- Notes

---

# 📌 Overview

This project provides a **voice-controlled smart home automation system powered by AI**.

The system allows users to:

- Control home devices using voice commands
- Use English or Arabic speech
- Monitor real-time sensor data
- Control devices remotely using a Flutter mobile app

## Technologies Used

|       Component       |     Technology     |
|-----------------------|--------------------|
|     Mobile App        |      Flutter       |
|   Backend Server      |   Flask (Python)   |
| AI Speech Recognition |   OpenAI Whisper   |
|    NLP Processing     |      MARBERTv2     |
|    Microcontroller    |        ESP32       |
|     Communication     |  HTTP + WebSocket  |
|    Authentication     |       Firebase     |

---

# 🏗 System Architecture


User Voice
     ↓
Flutter Mobile App
     ↓
Flask Backend Server
     ↓
Whisper Speech-to-Text
     ↓
NLP Command Processing
     ↓
ESP32 Smart Controller
     ↓
Home Devices & Sensors
     ↓
WebSocket Updates → Mobile App


---

# ✨ Features

## 🎤 Voice Control

- English voice commands
- Arabic voice commands
- AI-based speech recognition using Whisper

## 📱 Mobile Application

- Built using Flutter
- Firebase authentication
- Real-time device status
- WebSocket updates

## 🤖 AI Processing

- Speech-to-text conversion
- Arabic NLP using MARBERTv2
- Command extraction

## 🏠 Smart Device Control

Control multiple home devices:

- Lights
- Fan
- Curtains
- Main Door
- Sensors monitoring

## 📡 Real-Time Monitoring

Sensors provide:

- Temperature
- Humidity
- Light intensity
- Door status
- Curtain status

---

# 🔌 Hardware Setup

## Controlled Devices

- Kitchen Light
- Bathroom Light
- Room Light
- Living Room Lights
- Fan
- Motorized Curtain
- Main Door (Servo Motor)

## Sensors

- DHT22 Temperature & Humidity Sensor
- LDR Light Sensor
- Door Sensor
- Curtain Position Sensor

## GPIO Connections

| Device                  | GPIO Pin(s)           |
|-------------------------|-----------------------|
| Kitchen Light           | GPIO 25               |
| Bathroom Light          | GPIO 26               |
| Room Light              | GPIO 33               |
| Living Room Light 1     | GPIO 32               |
| Living Room Light 2     | GPIO 0                |
| Living Room Light Boost | GPIO 12, 13           |
| Fan                     | GPIO 22               |
| Curtain Motor           | GPIO 19 (A1), 21 (A2) |
| Main Door Servo         | GPIO 14               |
| DHT22 Sensor            | GPIO 27               |
| LDR Sensor              | GPIO 2                |
| Door Sensor             | GPIO 4                |
| Curtain Sensor          | GPIO 5                |

---

# 🖥 Backend Server (Flask + AI)

The backend server handles **AI processing and device communication**.

## Main Responsibilities

- Receive voice input
- Convert speech to text
- Extract commands
- Send commands to ESP32
- Provide real-time data APIs

## Libraries Used

- openai-whisper
- transformers
- Flask
- Flask-CORS
- websocket-client
- nltk
- autocorrect
- pyarabic
- spellchecker

## API Endpoints

| Endpoint                 | Description                             |
|--------------------------|-----------------------------------------|
| `/process-command`       | Accepts English audio                   |
| `/process-command-ar`    | Accepts Arabic audio                    |
| `/get-temperature`       | Returns current temperature             |
| `/get-<room>-status`     | Returns device status by room           |

---

# 📱 Flutter Mobile Application

## Features

- Voice command recording
- Firebase authentication
- Real-time device monitoring
- Device control interface
- Bloc (Cubit) state management
- Multi-language support

---

# 🎤 Supported Voice Commands

Examples:

open light kitchen
close light kitchen

open light bathroom
close light bathroom

open fan
close fan

open light room
close light room

open light living room
close light living room

increase light living room
decrease light living room

open curtain
close curtain

open door
close door

Arabic commands are also supported.

---

# ⚙️ Setup Instructions

## 1️⃣ Run Flutter Mobile App

flutter --version
flutter pub get
flutter run

## 2️⃣ Run Backend Server

cd backend
pip install -r requirements.txt
python app.py

⚠ First run will download the **Whisper AI model**.

## 3️⃣ Upload ESP32 Firmware

Steps:

1. Open **Arduino IDE**
2. Select board:

ESP32 Dev Module

3. Install libraries:

- WiFi.h
- ESPAsyncWebServer.h
- AsyncTCP.h
- ArduinoJson.h
- ESP32Servo.h
- DHT.h

4. Upload firmware
5. Open Serial Monitor (115200 baud)

---

# ⚠️ CRITICAL CONFIGURATION (ACTION REQUIRED)

Before running the project you **MUST update the network configuration**.

## ESP32 WiFi Setup

File:

Firmware/main.cpp

Edit:

const char* ssid = "YOUR_WIFI_NAME"; (Line 10)
const char* password = "YOUR_WIFI_PASSWORD"; (Line 11)

## Flutter App Server IP

Update the backend IP inside:

lib/shared/cubits/devicesstatus/devices_status_cubit.dart (Line 11)
lib/shared/cubits/home_screen/home_screen_cubit.dart (Line 21)
lib/shared/network/remote/dio_helper/dio_helper.dart (Line 8)

Example:

const String baseUrl = "http://192.168.1.100:5000";

## Backend ESP32 Configuration

File:

backend/app.py

Update ESP32 IP:

ESP_WS_IP = "ws://YOUR_ESP32_IP/ws" (Line 31)
ESP_HTTP_IP = "http://YOUR_ESP32_IP/control" (Line 32)

Update serial port:

esp = serial.Serial(port="YOUR_COM_PORT", baudrate=115200, timeout=1) (Line 40)

---

# 🎥 Demo

## Emulator Demo

https://drive.google.com/file/d/1p09WGqWYprNvnuu02MHXdrvNlB6iOxWT/view

## Real Device Control

https://drive.google.com/file/d/1c0s_ETLgs2JWwm1Tl0_tVnAwQ5lPd7tK/view

---

# 🚀 Future Improvements

- Fingerprint Authentication
- Scheduled Automation
- Grouped Device Control
- Cloud Dashboard
- Smart Scene Automation

---

# 📌 Notes

- All components must be on the **same WiFi network**
- Curtain motor runs for **3 seconds** to avoid damage
- Sensor data updates **every second via WebSocket**
- Check hardware wiring if a device does not respond

---

💡 **Project Type:** AI + Smart Home System  
Built using **Flutter, Python, Whisper AI, and ESP32**
