#include <phyphoxBle.h> 


const int potPin = 35;

// variable for storing the potentiometer value
float voltage = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  PhyphoxBLE::start();                //Start the BLE server

}

void loop() {
  // Reading potentiometer value
  voltage = analogRead(potPin)*(-0.0277)+42.519;
  Serial.print("U(V),");
  Serial.println(voltage,1);
  PhyphoxBLE::write(voltage);
  delay(2);
}
