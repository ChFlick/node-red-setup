# node-red-setup

This is the node red setup of my personal IoT environment at home.

## Smart Socket Control

One flow controls the smart sockets. In this case TECKIN sockets are used. They are integrated using [node-red-contibr-tuya-smart](https://github.com/hgross/node-red-contrib-tuya-smart).

### Features

- On/Off buttons are controlled by the socket as they can be controlled from outside of the application
- Status message, weather the sockets are online or not using flow variables
- Power Off-Safety: It is not possible to deactivate a socket if there is to much current flowing
- Display of the values in a gauge (real time) and a chart (5 minute aggregation)

![Socket Flow](https://github.com/ChFlick/node-red-setup/blob/master/socketFlow.png "Socket Flow")

## Sensor control using MQTT

The second flow controls MQTT devices. MQTT is handled by a [Mosca MQTT Broker](https://flows.nodered.org/node/node-red-contrib-mqtt-broker).

### Features

- Read temperature and humidity
- Read gas concentration (currently not fully implemented)
- Display of the values in a gauge (real time) and a chart (5 minute aggregation)
- Controlling a LED (On/Off)
- Controlling a Display

![MQTT Flow](https://github.com/ChFlick/node-red-setup/blob/master/sensorFlow.png "Sensor Flow")

## UI

![Dashboard](https://github.com/ChFlick/node-red-setup/blob/master/dashboard.PNG "Dashboard")

## Sensor/Actuator Setup

The Sensor/Actuator system uses a Wemos with an ESP8266 microcontroller.  
For temperature and humidity, a DHT22 sensor is used, reading through digital input.  
For the natural gas concentration, a MQ-5 sensor is used, reading through analog input.  
The display is a 128x64 i2c SSD1306 display.  

![Sensor Actuator Setup Front](https://github.com/ChFlick/node-red-setup/blob/master/wemosSetupFront.jpg "Sensor Actuator Setup Front")
![Sensor Actuator Setup Back](https://github.com/ChFlick/node-red-setup/blob/master/wemosSetupBack.jpg "Sensor Actuator Setup Back")
