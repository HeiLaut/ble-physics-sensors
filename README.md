# BLE Sensors for physics lessons

## Goal

**Easy to use** and affordable sensor boxes for digital data acquisition are suitable for use in physics lessons by students or for demonstrations by teachers.

Each box can be assembled for less than 20 euros.

## Microcontroller and Sensors

All boxes are equipped with an ESP32, enabling them to communicate via BLE (Bluetooth Low Energy) or a serial connection.

I use the Wemos Lolin32Lite developement board because of its availibility, the battery connector and the low price.

Most of the sensors are using the I2C interface to communicate with the ESP32.

### Force Sensor

Sensor:

Load Cell and HX711 amplifier

### ToF Distance-Sensor

Sensor:

VL53L0X

or

VL53L1X

### Light Gate

Sensor:

Adafruit Break Beam Sensor 

or (cheaper)

modified lightgate/speed-sensor with lm393

### Magnetometer

Sensor:

LI2MDL

or

MLX90393

or

(Gy-271) low range 

### DC Multimeter

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
