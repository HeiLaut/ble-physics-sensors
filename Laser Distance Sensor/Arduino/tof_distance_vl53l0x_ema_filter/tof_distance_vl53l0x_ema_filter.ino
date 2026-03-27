/*
zu ändern: Bei Knopfdruck wird immer der Orstoffset gesetzt;Zeitoffset wird nur dann auf 0 gesetzt, wenn in Phyphox der Mülleimer gedrückt wird
*/
//Version 0.4

#include <Adafruit_VL53L0X.h>
#include <phyphoxBle.h>

#define BUTTON_PIN 27 //PIN for Offset-Button
#define SDA 14 //PIN for I2C SDA
#define SCL 12 //PIN for I2C SCL

#define BLENAME "Laser-Distanz_#6"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

float d_prev, t_prev,d_curr,t_curr;
float distance, velocity, offset, t_offset, t, readDist;
bool stopped = 0;
bool cleared = 0;
bool synced = 0;

#define EMA_ALPHA_DIST 0.4f
#define EMA_ALPHA_VEL  0.5f

float emaDistance = 0.0f;
float emaVelocity = 0.0f;
bool emaInitialized = false;

void setup() {
  //needed to select the scl and sda port for the lolin lite board Wire.begin(I2C_SDA, I2C_SCL)
  Wire.begin(SDA, SCL);
  //enable internal LED c:\Users\Heinrich\Nextcloud\Physik\DIY\Physics-Arduino-Sensors\Force Sensor\Arduino\load_cell_esp_phyphox\load_cell_esp_phyphox.ino
 
  Serial.begin(115200);
   // wait until serial port opens for native USB devices
  while (!Serial) {
    delay(1);
  }
  delay(2000);

  Serial.println("-----------------------------");
  Serial.println("Vl53l0X-Sensor Version 0.31");
  Serial.println("-----------------------------");

  delay(1000);
 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);


  //Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1){
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    }
  }
  lox.startRangeContinuous();
  offset = 0;
  t_offset = 0;

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  
  
  //phyphox setup

  PhyphoxBLE::start(BLENAME);
  // PhyphoxBLE::configHandler = &receivedData;
  PhyphoxBLE::experimentEventHandler = &newExperimentEvent; // declare which function should be called after receiving an experiment event 
  //PhyphoxBLE::printXML(&Serial);
  //Experiment
  PhyphoxBleExperiment entfernung;  //generate experiment on Arduino which plot random values

  entfernung.setTitle("Laser-Entfernungsmesser");
  entfernung.setCategory("Sensor-Boxen");
  entfernung.setDescription("IR-Laser Entfernungsmessung");

  //View
  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Graph");
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Einfach");  //Create a "view"
  PhyphoxBleExperiment::View thirdView;
  thirdView.setLabel("Geschwindigkeit");  //Create a "view"



  //Graph
  PhyphoxBleExperiment::Graph firstGraph;
  firstGraph.setLabel("s(t)");
  firstGraph.setUnitX("s");
  firstGraph.setUnitY("cm");
  firstGraph.setLabelX("Zeit");
  firstGraph.setLabelY("Entfernung");
  //firstGraph.setStyle(STYLE_DOTS);


  firstGraph.setChannel(1, 2);

  PhyphoxBleExperiment::Graph secondGraph;  //Create graph which will plot random numbers over time
  secondGraph.setLabel("v(t)");
  secondGraph.setUnitX("s");
  secondGraph.setUnitY("cm/s");
  secondGraph.setLabelX("Zeit");
  secondGraph.setLabelY("Geschwindigkeit");
  secondGraph.setColor("308ead");
 // secondGraph.setStyle(STYLE_DOTS);
  //secondGraph.setStyle("dots");

  secondGraph.setChannel(1, 3);

  PhyphoxBleExperiment::Value dist;  //Creates a value-box.
  dist.setLabel("s");                //Sets the label
  dist.setPrecision(1);              //The amount of digits shown after the decimal point.
  dist.setUnit("cm");                //The physical unit associated with the displayed value.
  dist.setColor("FFFFFF");           //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  dist.setChannel(2);
  dist.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value vel;  //Creates a value-box.
  vel.setLabel("v");                //Sets the label
  vel.setPrecision(0);              //The amount of digits shown after the decimal point.
  vel.setUnit("cm/s");              //The physical unit associated with the displayed value.
  vel.setColor("308ead");           //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  vel.setChannel(3);
  vel.setXMLAttribute("size=\"2\"");

  //Export
  PhyphoxBleExperiment::ExportSet mySet;  //Provides exporting the data to excel etc.
  mySet.setLabel("Distanz");

  PhyphoxBleExperiment::ExportData exTime;
  exTime.setLabel("t(s)");
  exTime.setDatachannel(1);

  PhyphoxBleExperiment::ExportData exDist;
  exDist.setLabel("s(cm)");
  exDist.setDatachannel(2);

  PhyphoxBleExperiment::ExportData exVel;
  exVel.setLabel("v(cm/s)");
  exVel.setDatachannel(3);


  firstView.addElement(dist);
  firstView.addElement(firstGraph);  //attach graph to view
  //firstView.addElement(myEdit);

  secondView.addElement(dist);
  
  thirdView.addElement(dist);
  thirdView.addElement(firstGraph);
  thirdView.addElement(vel);
  thirdView.addElement(secondGraph);  //attach second graph to view

  entfernung.addView(firstView);   //Attach view to experiment
  entfernung.addView(secondView);  //Attach view to experiment
  entfernung.addView(thirdView);   //Attach view to experiment

  mySet.addElement(exTime);
  mySet.addElement(exDist);
  mySet.addElement(exVel);
  entfernung.addExportSet(mySet);

  PhyphoxBLE::addExperiment(entfernung);  //Attach experiment to server

}


void loop() {
int buttonState = digitalRead(BUTTON_PIN);
if (!buttonState) {
  Serial.println("press");
  delay(200);
  while (!lox.isRangeComplete()) {
    delay(1);
  }
  offset = lox.readRange() * 0.1;
  if (stopped) {
    cleared = 1;
  }
  // EMA bei neuem Offset zurücksetzen
  emaInitialized = false;
  Serial.println(offset);
}

  if (lox.isRangeComplete() && (!stopped || !synced)) {
  readDist = lox.readRange() * 0.1;
  distance = readDist - offset;

  float t = 0.001f * (float)millis() - t_offset;

  // Rohe Distanz per EMA filtern
  if (!emaInitialized) {
    emaDistance = distance;
  } else {
    emaDistance = EMA_ALPHA_DIST * distance + (1.0f - EMA_ALPHA_DIST) * emaDistance;
  }

  // Verschiebe Ringpuffer (auf gefilterter Distanz)
  d_prev = d_curr;
  t_prev = t_curr;
  d_curr = emaDistance;
  t_curr = t;

  // Rohe Geschwindigkeit aus geglätteten Distanzen
  float rawVelocity = (d_curr - d_prev) / (t_curr - t_prev);
  // Geschwindigkeit per EMA filtern
  if (!emaInitialized) {
    emaVelocity = 0.0f;
    emaInitialized = true;
  } else {
    emaVelocity = EMA_ALPHA_VEL * rawVelocity + (1.0f - EMA_ALPHA_VEL) * emaVelocity;
  }

  velocity = (t < 0.15f) ? 0.0f : emaVelocity;
  distance = emaDistance;  // gefilterter Wert für weitere Verwendung

  if (distance > 120) {
    distance = 120;
  }

  if (velocity > 500) {
    velocity = 500;
  }
  if (velocity < -500) {
    velocity = -500;
  }
  //serial Outputs for the web-serial-plotter https://github.com/HeiLaut/web-serial-plotter
  if(synced==0){
  Serial.print("t(s)");
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.print("s(cm)");
  Serial.print(",");
  Serial.print(distance);
  Serial.print(",");
  Serial.print("v(cm/s)");
  Serial.print(",");
  Serial.println(velocity);
  }

  PhyphoxBLE::write(t, distance, velocity);
  }
}
void newExperimentEvent(){
  Serial.println(PhyphoxBLE::eventType);
  if(PhyphoxBLE::eventType==0){
    Serial.println("Pause");
    stopped = 1;
  }
  
  if(PhyphoxBLE::eventType==1){
    Serial.println("Start");
    if(cleared){
      t_offset = 0.001f * (float)millis();
      d_prev = d_curr = t_prev = t_curr = 0;
      emaDistance = emaVelocity = velocity = 0.0f;
      emaInitialized = false;
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

//void receivedData() {           // get data from PhyPhox app
//  float readInput;
//  PhyphoxBLE::read(readInput);
//  intervall = readInput;
//}
