#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_INA219.h>


Adafruit_ADS1015 ads1015;
Adafruit_INA219 ina219;

void setup(void)
{ int sda = 15;
  int scl = 13;
  Wire.begin(sda, scl);
  Serial.begin(115200);
  Serial.println("Hello!");
  
  Serial.println("Getting differential reading from AIN0 (P) and AIN1 (N)");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV)");
  ads1015.begin(0x4A);
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
}

void loop(void)
{
  int16_t results;
  float t = 0.001 * (float)millis();
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  results = ads1015.readADC_Differential_0_1();
  Serial.print("t,");Serial.print(t);
  Serial.print(",V,");Serial.print(results * 3/0.091/1000);
  Serial.print(",A,");Serial.println(current_mA); 

  delay(10);
}