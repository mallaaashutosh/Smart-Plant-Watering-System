# 🌱 Smart Plant Watering System (IoT)

**An automatic plant watering system using ESP32, soil moisture sensor, DHT22, and relay pump control — with real-time monitoring via Node-RED dashboard.**

![Project Overview](https://via.placeholder.com/800x400.png?text=Smart+Plant+Watering+System)  
*(Replace this with your own photo/screenshot of the hardware + dashboard)*

## ✨ Features

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

## 📸 Dashboard Preview

![Dashboard](https://via.placeholder.com/800x450.png?text=Node-RED+Dashboard+with+Gauges)  
*Three-sensor gauges + pump status indicator*

## 🛠️ Hardware Used

| Component              | Model / Description                  | Pin on ESP32 |
|------------------------|--------------------------------------|--------------|
| Microcontroller        | ESP32 DevKit                         | —            |
| Soil Moisture Sensor   | YL-38 / Capacitive (AO analog)       | GPIO 34      |
| Temperature & Humidity | DHT22                                | GPIO 4       |
| Water Pump Relay       | 5V Relay Module (active LOW)         | GPIO 18      |

## 🔌 Software Stack

- **Firmware**: Arduino IDE + ESP32 board support
- **Libraries**:
  - WiFi.h
  - PubSubClient (MQTT)
  - DHT sensor library
- **Monitoring & UI**: Node-RED + node-red-dashboard
- **Communication**: MQTT over WiFi (public broker)

## 🚀 How It Works

1. ESP32 reads soil moisture, temperature, and humidity every 3 seconds
2. If soil moisture < 35% → pump turns ON
3. If soil moisture > 45% → pump turns OFF
4. All values are published to MQTT topics
5. Node-RED subscribes → displays beautiful gauges & status

## 📊 Dashboard Layout
