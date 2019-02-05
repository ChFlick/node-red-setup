# node-red-setup
This is the node red setup of my personal IoT environment at home.

# Smart Socket Control
One flow controls the smart sockets. In this case TECKIN sockets are used. They are integrated using [node-red-contibr-tuya-smart](https://github.com/hgross/node-red-contrib-tuya-smart).

## Features
- On/Off buttons are controlled by the socket as they can be controlled from outside of the application
- Status message, weather the sockets are online or not using flow variables
- Power Off-Safety: It is not possible to deactivate a socket if there is to much current flowing
- Display of the values in a gauge (real time) and a chart (5 minute aggregation)

![Socket Flow](https://github.com/ChFlick/node-red-setup/blob/master/socketFlow.png "Socket Flow")


# Sensor control using MQTT
The second flow controls MQTT devices. MQTT is handled by a [Mosca MQTT Broker](https://flows.nodered.org/node/node-red-contrib-mqtt-broker).

## Features
- Display of the values in a gauge (real time) and a chart (5 minute aggregation)
- Controlling an actuator (simply a LED).

![MQTT Flow](https://github.com/ChFlick/node-red-setup/blob/master/sensorFlow.png "Sensor Flow")

# UI
![Dashboard](https://github.com/ChFlick/node-red-setup/blob/master/dashboard.PNG "Dashboard")

# Sensor/Actuator Setup
The Sensor/Actuator system uses a Wemos with an ESP8266 microcontroller.
For temperature and humidity, a DHT22 sensor is being used.

![Sensor Actuator Setup](https://github.com/ChFlick/node-red-setup/blob/master/wemosSetup.jpg "Sensor Actuator Setup")
