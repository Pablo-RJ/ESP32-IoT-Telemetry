# ESP32 IoT Telemetry Node
![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-orange.svg)
![Protocol](https://img.shields.io/badge/protocol-MQTT-green.svg)
![Docker](https://img.shields.io/badge/docker-%230db7ed.svg?style=flat&logo=docker&logoColor=white)
![Node-RED](https://img.shields.io/badge/Node--RED-%238F0000.svg?style=flat&logo=nodered&logoColor=white)
## Overview
This project implements a secure, full-stack **IoT Edge Node** architecture. It captures environmental data (Temperature & Humidity) via an **ESP32** and transmits it wirelessly using the **MQTT** protocol to a containerized backend.

The system is designed with a focus on **scalability** and **security best practices**, featuring decoupled credentials, non-blocking I/O, Deep Sleep power optimization, and a Dockerized infrastructure ready for deployment.

## Key Features
* **Protocol:** MQTT (Lightweight Pub/Sub messaging).
* **Data Format:** JSON Serialization (`{"device": "id", "temp": 24.0...}`) for interoperability with Cloud services.
* **Resilience:** Automatic Wi-Fi and MQTT reconnection logic.
* **Security:** Credentials decoupled via `secrets.h` (GitOps best practice) and sensitive tokens excluded from version control.
* **Energy Efficiency:** Deep Sleep mode integration (10s sleep cycle) for battery-powered operation.
* **Infrastructure as Code:** Full backend stack (Broker + Logic) defined in `docker-compose.yml`.

## Project Structure
```text
/ESP32-IoT-Telemetry
├── /backend                    # Server-side configuration
│   ├── /node_red_data          # Logic flows & settings (Pre-configured)
│   └── mosquitto.conf          # MQTT Broker configuration
├── /firmware
│   └── /Edge_Telemetry_Node    # ESP32 C++ Firmware
│       ├── Edge_Telemetry_Node.ino
│       ├── WifiUtils.h
│       └── secrets_example.h
├── docker-compose.yml          # Container orchestration script
└── README.md

## Hardware Requirements
* **Microcontroller:** ESP32 DevKit V1.
* **Sensor:** DHT11 (Temperature & Humidity).
* **Connection:** DHT11 Data Pin connected to GPIO 4.

## Setup & Installation

1.  **Clone or Download** this repository.
2.  **Configuration:**
    * Navigate to `/firmware/Edge_Telemetry_Node/`.
    * Rename `SecretsExample.h` to `secrets.h`.
    * Open `secrets.h` and enter your Wi-Fi SSID, Password, and MQTT Broker IP.
3.  **Deploy:**
    * Open `Edge_Telemetry_Node.ino` in Arduino IDE.
    * Install required libraries: `PubSubClient` and `DHT sensor library`.
    * Upload the firmware to the ESP32.

## 🐳 Backend Deployment (Docker)

This project includes a fully containerized infrastructure using **Docker Compose**. It automatically sets up the MQTT Broker and the Logic Layer (Node-RED).

### Prerequisites
* [Docker Desktop](https://www.docker.com/products/docker-desktop/) installed.

### Quick Start
1. Navigate to the project root directory.
2. Run the stack:
   ```bash
   docker-compose up -d

 ## Future Improvements (Roadmap)
* [ ] Implementation of TLS/SSL (MQTTS) for encrypted communication.
* [x] Deep Sleep mode integration for battery optimization.
* [ ] Integration with a Time Series Database (InfluxDB) and Grafana.

## Credits & Acknowledgements
* **Lead Maintainer:** [Pablo Rodriguez Jimenez](https://github.com/Pablo-RJ) - Currently developing version 2.0+ (Security refactor & Cloud Integration).
* **Original Co-author:** [Pedro Alfonso Quiles García](https://github.com/pquigar) - Collaborated on the initial prototype logic for the university course.
---
*Open Source Project for Educational Purposes*
