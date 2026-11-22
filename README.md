# BLE Sensors for Physics Classes

* [Goal](#goal)
* [Microcontroller and Sensors](#sensors)
   * [Force Sensor](#force)
   * [ToF Distance Sensor](#tof)
   * [Light Gate](#gate)
   * [Magnetometer](#magnet)
   * [DC Multimeter](#multimeter)
   * [Barometer](#barometer)

## Goal <a name="goal"></a>

The goal is to provide **easy-to-use**, affordable sensor boxes for digital data acquisition, suitable both for student experiments and for teacher demonstrations in physics lessons.

Each box can be assembled for less than €20.

Feedback and suggestions for improving the 3D files or the Arduino code are welcome.

## Microcontroller and Sensors <a name="sensors"></a>

All boxes are equipped with an ESP32, enabling them to communicate via BLE (Bluetooth Low Energy) or a serial connection. Measurements are displayed and recorded with the free [phyphox](https://phyphox.org) app.

I use the LOLIN32 Lite development board because it is widely available, inexpensive, and has a built-in battery connector.

Most sensors use the I²C interface to communicate with the ESP32.

### Force Sensor <a name="force"></a>
<img src="Force%20Sensor/Pictures/Sensor.jpg" width="300">
<img src="Force%20Sensor/Pictures/phyphox.jpg" width="150">

Sensor:

Load cell and HX711 amplifier

### ToF Distance Sensor <a name="tof"></a>
<img src="Laser%20Distance%20Sensor/Pictures/Sensor.jpg" width="300">
<img src="Laser%20Distance%20Sensor/Pictures/phyphox.jpg" width="150">

Sensor:

VL53L0X

or

VL53L1X

[Assembly instructions: see wiki](https://github.com/HeiLaut/ble-physics-sensors/wiki/Laser-Distance)

### Light Gate <a name="gate"></a>
<img src="Light%20Gate/Pictures/Sensor.jpg" width="300">
<img src="Light%20Gate/Pictures/phyphox.jpg" width="150">

Sensor:

Adafruit IR break beam sensor

or (cheaper)

modified LM393 light gate / speed sensor module

### Magnetometer <a name="magnet"></a>
<img src="Magnetometer/Pictures/Sensor.jpg" width="300">
<img src="Magnetometer/Pictures/phyphox.jpg" width="150">

Sensor:

LIS2MDL

or

MLX90393

or

GY-271 (low range)

### DC Multimeter <a name="multimeter"></a>
<img src="Multimeter/Pictures/Sensor.jpg" width="300">
<img src="Multimeter/Pictures/phyphox.jpg" width="150">

Sensor:

INA219

and (optional)

ADS1115

Example videos:

I–V characteristic curve of a diode: https://youtu.be/DQ5L0hsJfBA

I–V characteristic curve of a resistor: https://youtu.be/PSxBtcULFwU

Charging and discharging a capacitor: https://youtube.com/shorts/HkM0g7PQ94s

### Barometer <a name="barometer"></a>
<img src="Pressure%20Sensor/Pictures/Sensor.jpg" width="300">
<img src="Pressure%20Sensor/Pictures/phyphox.jpg" width="150">

Sensor:

BMP180 barometer and thermometer module
