#include <phyphoxBle.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---- Pin-Konfiguration ----
#define SIGNAL_PIN 26   // aus Sketch 2 übernommen (Sketch 1 nutzte Pin 5)
#define BUTTON_PIN 27
#define SDA_PIN 16      // umbenannt, da SDA/SCL im ESP32-Core bereits belegt sind
#define SCL_PIN 17

// ---- OLED ----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int n = 0;
int t1 = 0;
int t2 = 0;
int timeArray[3] = {0,0,0};
int n_puffer = 0; // puffer to check, if a change happend.

float verdT = 0;
float laufT = 0;
float pendelT = 0;
float pendelF = 0;

float radius = 0;

// ---- Mode/Taster ----
// 0 = Laufzeit, 1 = Verdunklung, 2 = Pendel  (Mehrfachmessung/mode 3 entfernt)
int mode = 0;

void isr1() {
  n++;
  if(digitalRead(SIGNAL_PIN)){
    t1=(int)millis();
  }else{
    t2=(int)millis();
  }
}

void setup() {
  PhyphoxBLE::start("Einzel-Lichtschranke");
  PhyphoxBLE::setMTU(48); //6 float values 6*4 = 24 bytes
  // An extra task takes care of the experiment creation
  xTaskCreate(
    generateExperiment,
    "experimentTask",
    16000,
    NULL,
    1,
    NULL
  );

  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }

  pinMode(SIGNAL_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN), isr1, CHANGE);
}

void loop() {

  // ---- Taster: Moduswechsel ----
  if(!digitalRead(BUTTON_PIN)){
    mode++;
    mode %= 4;
    delay(100); // einfaches Entprellen, wie in Sketch 2
  }

  // Gets the current runtime in seconds
  float t = 0.001 * (float)millis();

  // gets darkening Time of the sensor
  verdT = abs((float)t1 - (float)t2) * 0.001;

  // Measures the time between t1 and the last two rising timestamps
  if(t1!=timeArray[2]){
    for(int i = 0; i<2 ;i++){
      timeArray[i] = timeArray[i+1];
    }
    timeArray[2]=t1;
    laufT = (timeArray[2]-timeArray[1])*0.001;
    pendelT = (timeArray[2]-timeArray[0])*0.001;
  }

  // calculates the frequency of a full pendulum swing
  pendelF = 1/pendelT;

  if(n_puffer != n){
    float values[6] = {t,laufT,verdT,pendelT,pendelF,(float)n};
    PhyphoxBLE::write(&values[0], 6);
    Serial.print("t,");Serial.print(t,3);
    Serial.print(",Laufzeit,");Serial.print(laufT,3);
    Serial.print(",Verdunklungszeit,"); Serial.print(verdT,3);
    Serial.print(",Schwingungsdauer,");Serial.print(pendelT,3);
    Serial.print(",Frequenz,");Serial.print(pendelF,3);
    Serial.print(",n,");Serial.println(n);
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
    oled.setCursor(5,10); oled.print(verdT,3); oled.print(" s");
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

  delay(50);
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
  verd.setLabel("Verdunklungszeit t =");
  verd.setPrecision(3);
  verd.setUnit("s");
  verd.setColor("FFCC5C");
  verd.setChannel(3);
  verd.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value laufz;
  laufz.setLabel("Signallaufzeit t =");
  laufz.setPrecision(3);
  laufz.setUnit("s");
  laufz.setColor("FFCC5C");
  laufz.setChannel(2);
  laufz.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value schwingd;
  schwingd.setLabel("Schwingungsdauer T =");
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
  periodGraph.setColor("76a5af");
  periodGraph.setChannel(6, 4);

  simple.addElement(verd);
  simple.addElement(laufz);

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

void receivedData() {           // get data from PhyPhox app
  float readInput;
  PhyphoxBLE::read(readInput);
   if(readInput>0){
      radius = readInput;
    }
}
