/*
 * Title: Einzellichtschranke
 * Version: 1.0
 * Author: Heinrich Lauterbach
 * GitHub: https://github.com/HeiLaut/ble-physics-sensors
 *
 * License (own code): MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, subject to the following condition:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Libraries used:
 * - phyphoxBLE (LGPL 3.0) - https://github.com/phyphox/phyphox-arduino
 * - Adafruit_GFX (BSD License) - https://github.com/adafruit/Adafruit-GFX-Library
 * - Adafruit_SSD1306 (BSD License) - https://github.com/adafruit/Adafruit_SSD1306
 */


#include <phyphoxBle.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---- Pin-Konfiguration ----
#define SIGNAL_PIN 26  
#define BUTTON_PIN 27
#define SDA_PIN 16   
#define SCL_PIN 17

// ---- OLED ----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define NAME "Einzel-Lichtschranke A"

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

volatile int n = 0;
volatile int t1 = 0;
volatile int t2 = 0;
volatile int timeArray[3] = {0, 0, 0};
volatile float laufT = 0;
volatile float pendelT = 0;
volatile float pendelF = 0;
volatile bool newEvent = false;
volatile unsigned long lastRise = 0;
const unsigned long DEBOUNCE_US = 2000; // Startwert, ggf. anpassen

float verdT = 0;
int n_puffer = 0;
bool stopped = 0;
bool cleared = 0;
bool synced = 0;

volatile float t = 0;
volatile float t_offset = 0;
// ---- Mode/Taster ----
// 0 = Laufzeit, 1 = Verdunklung, 2 = Pendel

int mode = 0;
bool lastButtonState = HIGH;   


void IRAM_ATTR isr1() {
  n++;
  if (digitalRead(SIGNAL_PIN)) {
    unsigned long now = micros();
    if (now - lastRise < DEBOUNCE_US) return;  // Prellen ignorieren, timeArray NICHT verschieben
    lastRise = now;
    t1 = (int)now;
    timeArray[0] = timeArray[1];
    timeArray[1] = timeArray[2];
    timeArray[2] = t1;
    newEvent = true;
  } else {
    t2 = (int)micros();
  }
}

void setup() {
  PhyphoxBLE::start(NAME);
  PhyphoxBLE::setMTU(48); //6 float values 6*4 = 24 bytes
  
  xTaskCreate(
    generateExperiment,
    "experimentTask",
    16000,
    NULL,
    1,
    NULL
  );

  PhyphoxBLE::experimentEventHandler = &newExperimentEvent;

  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }

  pinMode(SIGNAL_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN), isr1, CHANGE);

  t_offset = 0;

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

}

void loop() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading == LOW && lastButtonState == HIGH) {
    mode++;
    mode %= 4;
    delay(50);
   }

  lastButtonState = reading;


  if(digitalRead(SIGNAL_PIN)){
    verdT = abs((float)t1 - (float)t2) * 0.000001f;
  }
  // Measures the time between t1 and the last two rising timestamps
 

 if (newEvent) {
    newEvent = false;
    t  = 0.000001f * (float)t1 - t_offset;
    laufT   = (timeArray[2] - timeArray[1]) * 0.000001f;
    pendelT = (timeArray[2] - timeArray[0]) * 0.000001f;
    pendelF = 1.0f / pendelT;

    float values[6] = {t,laufT,verdT,pendelT,pendelF,(float)floor(n/2)};
    PhyphoxBLE::write(&values[0], 6);
    Serial.print("t,");Serial.print(t,3);
    Serial.print(",Laufzeit,");Serial.print(laufT,3);
    Serial.print(",Verdunklungszeit,"); Serial.print(verdT,4);
    Serial.print(",Schwingungsdauer,");Serial.print(pendelT,3);
    Serial.print(",Frequenz,");Serial.print(pendelF,3);
    Serial.print(",n,");Serial.println(floor(n/2),0);
  }
  n_puffer = n;

  // ---- OLED-Ausgabe ----
  oled.clearDisplay();
  oled.setTextColor(WHITE);

  if(mode == 0){
    oled.setTextSize(1);
    oled.setCursor(5,0);  oled.print("Laufzeit");
    oled.setTextSize(2);
    oled.setCursor(5,10); oled.print(laufT,3); oled.print(" s");
  }
  if(mode == 1){
    oled.setTextSize(1);
    oled.setCursor(5,0);  oled.print("Verdunklung");
    oled.setTextSize(2);
    oled.setCursor(5,10); oled.print(verdT,4); oled.print(" s");
  }
  if(mode == 2){
    oled.setTextSize(1);
    oled.setCursor(5,0);  oled.print("Pendel T");
    oled.setTextSize(2);
    oled.setCursor(5,10); oled.print(pendelT,3); oled.print(" s");
  }
   if(mode == 3){
    oled.setTextSize(1);
    oled.setCursor(5,0);  oled.print("Pendel f");
    oled.setTextSize(2);
    oled.setCursor(5,10); oled.print(pendelF,3); oled.print(" Hz");
  }


  oled.setTextSize(1);
  oled.setCursor(120,0);
  oled.print(mode);
  oled.display();

  //delay(50);
}

void generateExperiment(void * parameter) {
  PhyphoxBleExperiment lightBarrier;

  lightBarrier.setTitle("Einzel-Lichtschranke");
  lightBarrier.setCategory("Sensor-Boxen");
  lightBarrier.numberOfChannels = 6;
  lightBarrier.setDescription("Lichtschranke");

  PhyphoxBleExperiment::View graph;
  graph.setLabel("Vielfachmessung");

  PhyphoxBleExperiment::View simple;
  simple.setLabel("Laufzeiten");

  PhyphoxBleExperiment::View multi;
  multi.setLabel("Pendel");

  PhyphoxBleExperiment::Graph nGraph;
  nGraph.setLabel("Ereignisse");
  nGraph.setUnitY("");
  nGraph.setUnitX("s");
  nGraph.setLabelX("Zeit t");
  nGraph.setLabelY("");
  nGraph.setColor("FFCC5C");
  nGraph.setChannel(1,6);

  PhyphoxBleExperiment::Value verd;
  verd.setLabel("Verd.zeit t =");
  verd.setPrecision(4);
  verd.setUnit("s");
  verd.setColor("FFCC5C");
  verd.setChannel(3);
  verd.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Graph verdGraph;
  verdGraph.setLabel("Verd.zeit");
  verdGraph.setUnitY("s");
  verdGraph.setUnitX("s");
  verdGraph.setLabelX("t");
  verdGraph.setLabelY("dT");
  verdGraph.setStyle(STYLE_DOTS);
  verdGraph.setLinewidth(2);
  verdGraph.setColor("76a5af");
  verdGraph.setChannel(1,3);

  PhyphoxBleExperiment::Value laufz;
  laufz.setLabel("Laufzeit t =");
  laufz.setPrecision(3);
  laufz.setUnit("s");
  laufz.setColor("FFCC5C");
  laufz.setChannel(2);
  laufz.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value schwingd;
  schwingd.setLabel("Schw.dauer T =");
  schwingd.setPrecision(3);
  schwingd.setUnit("s");
  schwingd.setColor("FFCC5C");
  schwingd.setChannel(4);
  schwingd.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value freq;
  freq.setLabel("Frequenz f =");
  freq.setPrecision(3);
  freq.setUnit("Hz");
  freq.setColor("FFCC5C");
  freq.setChannel(5);
  freq.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Graph periodGraph;
  periodGraph.setLabel("Schwingungsdauer");
  periodGraph.setUnitY("s");
  periodGraph.setUnitX("");
  periodGraph.setLabelX("n");
  periodGraph.setLabelY("T");
  periodGraph.setStyle(STYLE_DOTS);
  periodGraph.setLinewidth(2);
  periodGraph.setColor("76a5af");
  periodGraph.setChannel(6, 4);

  simple.addElement(verd);
  simple.addElement(laufz);

  graph.addElement(verdGraph);
  graph.addElement(nGraph);

  multi.addElement(schwingd);
  multi.addElement(freq);
  multi.addElement(periodGraph);

  lightBarrier.addView(simple);
  lightBarrier.addView(multi);
  lightBarrier.addView(graph);

  PhyphoxBLE::addExperiment(lightBarrier);

  vTaskDelete(NULL);
}


void newExperimentEvent(){
 
  if(PhyphoxBLE::eventType==1){
    Serial.println("Start");
    if(cleared){
      t_offset = 0.000001f * (float)micros();
      n = 0;
      n_puffer = 0;
    }
    stopped = 0;
    cleared = 0;
  }

  if(PhyphoxBLE::eventType==2){
    Serial.println("Clear");
    stopped = 1;
    synced  = 1;
    cleared = 1;
  }

  if(PhyphoxBLE::eventType==255){
    stopped = 1;
    synced  = 1;
    cleared = 1;
    Serial.println("synced");
  }
}