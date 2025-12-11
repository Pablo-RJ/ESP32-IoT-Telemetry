# ESP32 IoT Telemetry Node
![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-orange.svg)
![Protocol](https://img.shields.io/badge/protocol-MQTT-green.svg)
## Overview
This project implements a secure **IoT Edge Node** using an **ESP32** microcontroller. It captures environmental data (Temperature & Humidity) via a **DHT11** sensor and transmits it wirelessly using the **MQTT** protocol.

The system is designed with a focus on **scalability** and **security best practices**, featuring decoupled credentials, non-blocking I/O architecture, and standardized JSON payloads for easy Cloud/Backend integration.

## Key Features
* **Protocol:** MQTT (Lightweight Pub/Sub messaging).
* **Data Format:** JSON Serialization (`{"device": "id", "temp": 24.0...}`) for interoperability with Node-RED, AWS IoT, or Azure.
* **Resilience:** Automatic Wi-Fi and MQTT reconnection logic.
* **Security:** Credentials are decoupled from the source code using a `secrets.h` file pattern (GitOps best practice).
* **Energy Efficiency;** Deep Sleep mode integration (10s sleep cycle) for battery-powered operation.
## Project Structure
```text
/firmware
  └── /Edge_Telemetry_Node
       ├── Edge_Telemetry_Node.ino  # Main application logic
       ├── WifiUtils.h              # Network connection helper
       ├── secrets_example.h        # Template for credentials (Public)
       └── secrets.h                # (Ignored by Git) Local credentials
````
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

 ## Future Improvements (Roadmap)
* [ ] Implementation of TLS/SSL (MQTTS) for encrypted communication.
* [x] Deep Sleep mode integration for battery optimization.
* [ ] Integration with a Time Series Database (InfluxDB) and Grafana.

## Credits & Acknowledgements
* **Lead Maintainer:** [Pablo Rodriguez Jimenez](https://github.com/Pablo-RJ) - Currently developing version 2.0+ (Security refactor & Cloud Integration).
* **Original Co-author:** [Pedro Alfonso Quiles García](https://github.com/pquigar) - Collaborated on the initial prototype logic for the university course.
---
*Open Source Project for Educational Purposes*
