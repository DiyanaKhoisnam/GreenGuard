#  Smart Greenhouse Automation System using Arduino & ESP8266

##  Project Overview

The Smart Greenhouse Automation System is an IoT-based automation project built using **Arduino UNO** and **ESP8266 NodeMCU**. The system continuously monitors environmental conditions inside a greenhouse and automatically controls irrigation, cooling, and lighting systems.

The project combines:
- Embedded Systems
- IoT Dashboarding
- Sensor Automation
- OLED Visualization
- Real-Time Monitoring

The ESP8266 hosts a wireless live dashboard while simultaneously updating an OLED display with real-time greenhouse data.

---

#  Features

##  Environmental Monitoring
The system continuously monitors:

- Temperature
- Humidity
- Soil Moisture
- Light Intensity
- Rain Detection

---

#  Automation Features

##  Smart Irrigation System
The water pump automatically turns ON when:
- Soil becomes dry
- No rain is detected

The pump automatically turns OFF when:
- Soil becomes wet
- Rain is detected

---

##  Automatic Cooling System
The cooling fan automatically turns ON when:
- Temperature exceeds threshold value

The fan automatically turns OFF when temperature normalizes.

---

##  Smart Grow Light
The grow light automatically turns ON when:
- The environment becomes dark

The grow light automatically turns OFF when sufficient light is available.

---

#  IoT Dashboard Features

The ESP8266 hosts a live web dashboard accessible through Wi-Fi.

The dashboard displays:

- Temperature
- Humidity
- Soil Status
- Light Condition
- Rain Status
- Pump Status
- Fan Status
- Grow Light Status
- Smart Insights

The dashboard updates automatically in real time without refreshing the page.

---

#  OLED Display Features

The OLED display connected to ESP8266 shows:

- Temperature
- Humidity
- Soil Condition
- Rain Condition
- Pump Status
- Fan Status
- Grow Light Status

This allows local monitoring even without opening the dashboard.

---

# System Architecture

```text
Sensors
   ↓
Arduino UNO
   ↓
Automation Logic
   ↓
Serial Communication
   ↓
ESP8266 NodeMCU
   ↓
Wi-Fi Dashboard + OLED Display
```

---

#  Hardware Components

## Controllers
- Arduino UNO
- ESP8266 NodeMCU

---

## Sensors
- DHT11 Temperature & Humidity Sensor
- Soil Moisture Sensor
- LDR Sensor
- Rain Sensor

---

## Output Devices
- Relay Module
- Water Pump
- DC Fan
- Grow Light / LED
- OLED SSD1306 Display

---

## Miscellaneous
- Breadboard
- Jumper Wires
- USB Cables
- Resistors
- Power Supply

---

#  Pin Connections

## Arduino UNO Connections

| Component | Pin |
|---|---|
| DHT11 DATA | D4 |
| Soil Moisture Sensor | A0 |
| LDR Sensor | A1 |
| Rain Sensor | A2 |
| Pump Relay | D8 |
| Fan Relay | D9 |
| Grow Light Relay | D10 |
| ESP TX | D2 |
| ESP RX | D3 |

---

## OLED Connections (ESP8266)

| OLED Pin | ESP8266 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | D2 |
| SCL | D1 |

---

# Important Connection Note

ESP8266 RX works only on **3.3V**.

Arduino UNO TX outputs **5V**.

A voltage divider or logic level shifter must be used between:

```text
Arduino TX → ESP RX
```

---

#  Software Requirements

## Arduino IDE
Download:
https://www.arduino.cc/en/software

---

#  Required Libraries

## Arduino UNO Libraries

Install:
- DHT Sensor Library
- SoftwareSerial

---

## ESP8266 Libraries

Install:
- ESP8266WiFi
- ESP8266WebServer
- U8g2
- Wire

---

#  Installing ESP8266 Board in Arduino IDE

## Step 1
Open Arduino IDE

---

## Step 2
Go to:

```text
File → Preferences
```

---

## Step 3
Add this URL inside "Additional Boards Manager URLs":

```text
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

---

## Step 4
Open:

```text
Tools → Board → Boards Manager
```

---

## Step 5
Search:

```text
ESP8266
```

---

## Step 6
Install ESP8266 Board Package

---

#  Uploading the Code

## Upload Arduino Code

1. Connect Arduino UNO
2. Select correct COM port
3. Upload Arduino code

---

## Upload ESP8266 Code

IMPORTANT:
Disconnect TX/RX wires before uploading ESP code.

Then:

1. Select board:

```text
NodeMCU 1.0 (ESP-12E Module)
```

2. Select correct COM port
3. Upload ESP8266 code

Reconnect TX/RX wires afterward.

---

#  Accessing the Dashboard

## Step 1
Power ON both boards.

---

## Step 2
Connect phone/laptop to Wi-Fi:

```text
SSID: Greenhouse
Password: 12345678
```

---

## Step 3
Open browser:

```text
http://192.168.4.1
```

---

#  Dashboard Features

## Sensor Cards
- Temperature
- Humidity
- Soil Moisture
- Light Condition
- Rain Status

---

## Device Status
- Pump ON/OFF
- Fan ON/OFF
- Grow Light ON/OFF

---

## Smart Insights
Examples:
- Plants require irrigation
- High temperature detected
- Rainfall detected
- Irrigation system active
- Soil moisture healthy

---

#  OLED Display Interface

Example OLED Output:

```text
SMART GREENHOUSE

T:29C      H:65%
SOIL:WET   PUMP:OFF
LIGHT:OK   FAN:ON
RAIN:NO    LED:OFF
```

---

#  Automation Logic

## Irrigation Logic

```text
IF soil is dry
AND no rain detected
THEN turn pump ON
```

---

## Fan Logic

```text
IF temperature > threshold
THEN turn fan ON
```

---

## Grow Light Logic

```text
IF environment is dark
THEN turn grow light ON
```

---

#  Data Flow

```text
Sensors
   ↓
Arduino reads sensor values
   ↓
Arduino performs automation logic
   ↓
Arduino sends data to ESP8266
   ↓
ESP8266 updates:
   • Dashboard
   • OLED Display
```

---

#  Serial Monitor Debugging

## Arduino Serial Output

Displays:
- Sensor values
- Device statuses
- Automation activity
- Data sent to ESP8266

---

## ESP8266 Serial Output

Displays:
- Raw serial data
- Parsed values
- Wi-Fi IP address
- Dashboard server status

---

#  Project Highlights

- IoT-based monitoring
- Real-time automation
- Wireless dashboard
- OLED visualization
- Smart irrigation
- Modern responsive UI
- AJAX live updates
- Embedded intelligence

---

# Future Improvements

Possible future upgrades:

- Firebase integration
- Mobile app
- Blynk support
- Cloud analytics
- Weather API integration
- AI crop prediction
- Historical graphs
- MQTT communication
- Solar-powered system
- Camera monitoring

---

# Applications

- Smart Farming
- Precision Agriculture
- Greenhouse Automation
- Hydroponics
- Educational IoT Projects
- Sustainable Agriculture

---

# 🧪 Troubleshooting

## ESP8266 Upload Error

If upload fails:

- Disconnect TX/RX wires
- Check COM port
- Select correct board
- Press FLASH/RESET if required

---

## Dashboard Not Opening

Check:
- Connected to Greenhouse Wi-Fi
- Open:

```text
http://192.168.4.1
```

- Allow firewall permissions

---

## Wrong Sensor Values

Check:
- Sensor wiring
- Threshold values
- Power supply
- Ground connections

---

#  Technologies Used

- Arduino UNO
- ESP8266 NodeMCU
- Embedded C++
- HTML
- CSS
- JavaScript
- AJAX
- U8g2 Graphics Library
- IoT Web Server

---

#  Learning Outcomes

This project demonstrates:

- Sensor interfacing
- Embedded automation
- Serial communication
- IoT dashboard development
- OLED graphics programming
- Real-time web updates
- Smart agriculture systems

---

#  Acknowledgements

Built as an IoT Smart Agriculture and Greenhouse Automation project using Arduino UNO and ESP8266 NodeMCU.
