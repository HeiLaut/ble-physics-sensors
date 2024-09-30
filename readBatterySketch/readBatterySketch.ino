//phyphox Bibliothek wird geladen
#include <phyphoxBle.h>

// GPIO 35 ist ein analoger Pin auf dem ESP32
const int analogPin = 4;
int oldMillis = 0;
int blinkInt = 200;
bool ledStatus = 0;
//setup() wird einmal beim Start des MC aufgerufen
void setup() {
  //Start the BLE server
  PhyphoxBLE::start();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledStatus);
}

//loop() läuft in Dauerschleife bis das Gerät ausgeschaltet wird
void loop() {
  int t = millis();
  // Lese den analogen Wert vom definierten Pin
  float analogValue = analogRead(analogPin);

  if ((analogValue <2000) && ((t - oldMillis) >= blinkInt)) {
    oldMillis = t;
    ledStatus = !ledStatus;
    digitalWrite(LED_BUILTIN, ledStatus);
  }else{
    digitalWrite(LED_BUILTIN, LOW);

  }
  
  //Sende den analogen Wert an phyphox
  PhyphoxBLE::write(analogValue);
  //warte 50ms bis der nächste Wert gelesen wird
  delay(50);
}
