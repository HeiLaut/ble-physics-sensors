#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1015 ads1015;

void setup(void)
{
 // Wire.begin(13,12);
  Serial.begin(115200);
  Serial.println("Hello!");
  
  Serial.println("Getting differential reading from AIN0 (P) and AIN1 (N)");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV)");
  ads1015.begin();
}

void loop(void)
{
  int16_t results;

  results = ads1015.readADC_Differential_0_1();
  Serial.print("V,");Serial.println(results * 3/0.091/1000); 

  delay(1);
}