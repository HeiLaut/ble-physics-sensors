# BLE Sensors for physics lessons

## Goal

**Easy to use** and affordable sensor boxes for digital data acquisition are suitable for use in physics lessons by students or for teacher demonstrations.

Each box can be assembled for less than 20 euros.

## Microcontroller and Sensors

All boxes are equipped with an ESP32, enabling them to communicate via BLE (Bluetooth Low Energy) or a serial connection.

I use the Wemos Lolin32Lite development board because of its availability, the battery connector and the low price.

Most sensors use the I2C interface to communicate with the ESP32.

### Force Sensor
<img src="https://github.com/HeiLaut/ble-physics-sensors/blob/main/Force%20Sensor/Pictures/Sensor.jpg" width="300">
<img src="https://github.com/HeiLaut/ble-physics-sensors/blob/main/Force%20Sensor/Pictures/phyphox.jpg" width="150">
Sensor:

Load Cell and HX711 amplifier

### ToF Distance-Sensor
<img src="https://github.com/HeiLaut/ble-physics-sensors/blob/main/Laser%20Distance%20Sensor/Pictures/Sensor.jpg" width="300">
<img src="https://github.com/HeiLaut/ble-physics-sensors/blob/main/Laser%20Distance%20Sensor/Pictures/phyphox.jpg" width="150">
Sensor:

VL53L0X

or

VL53L1X

### Light Gate

<img src="https://github.com/HeiLaut/ble-physics-sensors/blob/main/Light%20Gate/Pictures/Sensor.jpg" width="300">
<img src="https://github.com/HeiLaut/ble-physics-sensors/blob/main/Light%20Gate/Pictures/phyphox.jpg" width="150">


Sensor:

Adafruit Break Beam Sensor 

or (cheaper)

modified lightgate/speed-sensor with LM393

### Magnetometer
<img src="https://github.com/HeiLaut/ble-physics-sensors/blob/main/Magnetometer/Pictures/Sensor.jpg" width="300">
<img src="https://github.com/HeiLaut/ble-physics-sensors/blob/main/Magnetometer/Pictures/phyphox.jpg" width="150">


Sensor:

LI2MDL

or

MLX90393

or

(Gy-271) low range 

### DC Multimeter

<img src="https://github.com/HeiLaut/ble-physics-sensors/blob/main/Multimeter/Pictures/Sensor.jpg" width="300">

Sensor:

INA219

and (optional)

ADS1115

Example Videos:

Diode characteristic https://youtu.be/DQ5L0hsJfBA

Resistor characteristic https://youtu.be/PSxBtcULFwU

Charge and Discharge of a capacitor https://youtube.com/shorts/HkM0g7PQ94s?feature=share


### Barometer 

BMP180 Barometer and Thermometer module. 
