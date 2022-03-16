# Sensor boxes for physic lessons

## Goal

**Easy to use** and cheap sensorboxes for digital data acquisition that can be used in physic lessons by students or by the teacher as demonstration.

Each Box can be build for under 20 €.

All boxes are equiped with an esp32, so that they can communicate via BLE or serial connection. 

Just power up the sensor-box and take a smartphone with pyhphox installed, and you can start. Nothing more is needed.

If you want more, you can export the data from phyphox or read the data via serial connection on any pc and process them in the software you want.

## how to build

A detailed description how to build the sensors and use the software can be found in the wiki at this repository: 
https://github.com/HeiLaut/Physics-Arduino-Sensors/wiki

## Software

### Wireless via PhyPhox
<img src="https://github.com/HeiLaut/Physics-Arduino-Sensors/blob/main/images/us_example.jpg" width=25%>


It is planned that all models will be equipped with the ESP32 module. Using this platform, the measured values can then be read out directly via Bluetooth and PhyPhox (https://phyphox.org/de/home-de/) with the smartphone. The boxes then only need to be supplied with power. (power cable or power bank) 


### Python Serial Plotter
The sensor boxes can all be read out via a computer. For this purpose a Pyhton program is developed, which plots the respective measured value over time. The software has an export function so that the data can then be further processed on the PC.

## Boxes

### Force sensor

![Building instructions in the wiki](https://github.com/HeiLaut/Physics-Arduino-Sensors/wiki/Building-Instructions#1-force-gauge)

All files available.

Load cell to measure the force as a function of time.


### Ultrasonic-Rangefinder
![Building instructions in the wiki](https://github.com/HeiLaut/Physics-Arduino-Sensors/wiki/Building-Instructions#2-ultrasonic-rangefinder)

All files available. 

ultrasonic sensor - distance measurement as a function of time

### Laser-ToF-Rangefinder

Building instructions commnig soon

All files available.

Laser TimeOfFlight-Senosr - distance measurement as a function of time.

### Voltmeter
Building instructions commnig soon

(so far still realized with Ardino nano)

DC voltage measurement over a time interval.

# Sensoren für den Physikunterricht

## Ziel

**Konstengünstige** und einfach zu nutzende Sensor-Boxen für den Physikunterricht zur verfügung zu stellen, die im Physikunterricht von Schülern oder vom Lehrer zur Demonstration eingesetzt werden können.

Alle Boxen können für unter 20 € gebaut werden.

Alle Boxen sind mit einem esp32 ausgestattet, so dass sie über BLE oder eine serielle Verbindung kommunizieren können. 

Einfach die Sensor-Box einschalten und ein Smartphone mit installiertem pyhphox nehmen, und schon kann es losgehen. Mehr ist nicht nötig.

Wenn Sie mehr wollen, können Sie die Daten aus phyphox exportieren oder die Daten über eine serielle Verbindung auf einem beliebigen PC auslesen und in der gewünschten Software verarbeiten.


## Software


### Drahtlos über PhyPhox

Es ist geplant, dass alle Modelle mit dem ESP32 Modul ausgestattet werden. Über diese Plattform können die Messwerte dann direkt über Bluetooth und PhyPhox mit dem Smartphone ausgelesen werden. Die Boxen müssen dann lediglich mit Strom versorgt werden. (Netzkabel oder Powerbank)
https://phyphox.org/de/home-de/

### Python Serial Plotter

Die Sensorboxen können alle über einen Computer ausgelesen werden. Dazu wird ein Pyhton-Program entwickelt, welche den jeweiligen Messwert über die Zeit darstellt.
Die Software verfügt über eine Export-Funktion, so dass die Daten am PC dann weiter verarbeitet werden können.

## Boxen

### Kraftsensor

Alle Dateien vorhanden. (1. Version)

Wägezelle zur Messung der Kraft in Abhängigkeit von der Zeit

### Voltmeter 

(bisher noch mit Ardino nano realisiert)

Gleichspannungsmessung über ein Zeitintervall

### Ultraschall-Entfernungsmesser

Alle Dateien vorhanden. (1. Version)

Ultraschallsensor - Entfernungsmessung in Abhängigkeit von der Zeit

### Laser-Entfernungsmesser
