# Sensor boxes for physics lessons

## Goal

**Easy to use** and cheap sensor boxes for digital data acquisition that can be used in physic lessons by students or by the teacher as a demonstration.

Each Box can be built for under 20 €.

All boxes are equipped with an esp32, so that they can communicate via BLE or serial connection. 

Just power up the sensor-box and take a smartphone with pyhphox installed, and you can start. Nothing more is needed.

If you want more, you can export the data from phyphox or read the data via a serial connection on any pc and process them in the software you want.

## How to build

A detailed description of how to build the sensors and use the software can be found in the wiki at this repository: 
https://github.com/HeiLaut/Physics-Arduino-Sensors/wiki (work in progress)

## Software

### Wireless via PhyPhox
<img src="https://github.com/HeiLaut/Physics-Arduino-Sensors/blob/main/images/us_example.jpg" width=25%>


It is planned that all models will be equipped with the ESP32 module. Using this platform, the measured values can then be read out directly via Bluetooth and PhyPhox (https://phyphox.org/de/home-de/) with the smartphone. The boxes then only need to be supplied with power. (power cable or power bank) 

### Serial Plotter
#### Web Serial Plotter (Chrome-based browsers only)
I am working on a Web Serial Plotter, than can be accessed throughChrome or Edge. It automatically reads the data from the sensor. You can specify which data to plot and also output the data to a CSV-file. For the plotting i am using Plotly. Still in an early Version but usable.
You can find the serial plotter at the adress: https://heilaut.github.io/web-serial-plotter/

#### Python Serial Plotter
The sensors can also be read out via the serial interface of the computer. For this purpose, a Pyhton program is developed, which plots the respective measured value over time. The software has an export function so that the data can then be further processed on the PC. The program already works but is missing a few features.

## Boxes

### Force sensor
<img src="https://github.com/HeiLaut/Physics-Arduino-Sensors/blob/main/images/Photo.jpg" width = 50%>

[Building instructions --> wiki](https://github.com/HeiLaut/Physics-Arduino-Sensors/wiki/Building-Instructions#1-force-gauge)

All files are available.

Load cell to measure the force as a function of time.


### Ultrasonic-Rangefinder
<img src="https://github.com/HeiLaut/Physics-Arduino-Sensors/blob/main/images/distance_1.JPG" width = 50%>

[Building instructions --> wiki](https://github.com/HeiLaut/Physics-Arduino-Sensors/wiki/Building-Instructions#2-ultrasonic-rangefinder)

All files are available. 

ultrasonic sensor - distance measurement as a function of time

### Laser-ToF-Rangefinder
<img src="https://github.com/HeiLaut/Physics-Arduino-Sensors/blob/main/images/ToF-Rangefinder.jpg" width = 50%>
Building instructions commnig soon

All files available.

Laser TimeOfFlight-Sensor - distance measurement as a function of time.

### Light Gate 

Can be used as a single device or in series with an second gate or a magnetic holder to have an release timer.
Needs a little soldering effort to build it. 


### DC Multimeter
Building instructions coming soon

DC Multimeter. Measures Voltage up to 24V and current up to 3 A via an INA219 Module.

### Barometer 

BMP180 Barometer and Thermometer module. 
