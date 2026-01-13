#  Smart Plant Watering System (IoT)

**An automatic plant watering system using ESP32, soil moisture sensor, DHT22, and relay pump control — with real-time monitoring via Node-RED dashboard.**


##  Features

- **Automatic watering** based on soil moisture level (threshold + hysteresis to avoid rapid on/off)
- Real-time monitoring of:
  - Soil moisture (%)
  - Temperature (°C)
  - Humidity (%)
  - Pump state (ON/OFF)
- Beautiful **Node-RED dashboard** with gauges and status indicators
- MQTT communication using public broker (broker.mqttdashboard.com)
- Clean, well-documented Arduino code for ESP32
- Easy to extend (add manual control, alerts, history chart, etc.)

##  Dashboard Preview

<img width="1920" height="1150" alt="Screenshot (165)" src="https://github.com/user-attachments/assets/d2913f0b-7e98-4d07-8fc4-5b65862467a9" />



##  Hardware Used

| Component              | Model / Description                  | Pin on ESP32 |
|------------------------|--------------------------------------|--------------|
| Microcontroller        | ESP32 DevKit                         | —            |
| Soil Moisture Sensor   | YL-38 / Capacitive (AO analog)       | GPIO 34      |
| Temperature & Humidity | DHT22                                | GPIO 4       |
| Water Pump Relay       | 5V Relay Module (active LOW)         | GPIO 18      |

##  Software Stack

- **Firmware**: Arduino IDE + ESP32 board support
- **Libraries**:
  - WiFi.h
  - PubSubClient (MQTT)
  - DHT sensor library
- **Monitoring & UI**: Node-RED + node-red-dashboard
- **Communication**: MQTT over WiFi (public broker)

##  How It Works

1. ESP32 reads soil moisture, temperature, and humidity every 3 seconds
2. If soil moisture < 40% → pump turns ON
3. If soil moisture > 40% → pump turns OFF
4. All values are published to MQTT topics
5. Node-RED subscribes → displays beautiful gauges & status

##  How to Build Your Own

1. Wire the hardware as per pin definitions
2. Upload the ESP32 firmware (update WiFi credentials)
3. Import Node-RED flow (or build it manually)
4. Finally deploy


<div align="center">

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Node-RED](https://img.shields.io/badge/Node--RED-8BE35A?style=for-the-badge&logo=nodered&logoColor=black)](https://nodered.org/)
[![ESP32](https://img.shields.io/badge/ESP32-000000?style=for-the-badge&logo=espressif&logoColor=white)](https://www.espressif.com/)
[![MQTT](https://img.shields.io/badge/MQTT-3C6E9C?style=for-the-badge&logo=mqtt&logoColor=white)](https://mqtt.org/)

</div>
