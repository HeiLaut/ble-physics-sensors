#include <phyphoxBle.h>
#include "Adafruit_VL53L1X.h"

#define BUTTON_PIN 27
#define READBAT_PIN 25
#define SDA 16//15//16
#define SCL 17//13// 17

float d1, d2, d3, d4, d5, t1, t2, t3, t4, t5;
float duration, distance, d_old, velocity, offset, t_offset,readDist;
//float intervall = 20;
int stopped = 1;
int pressed = 0;
int synced = 0;
Adafruit_VL53L1X vl53 = Adafruit_VL53L1X();


void setup() {
  //needed to select the scl and sda port for the lolin lite board Wire.begin(I2C_SDA, I2C_SCL)
  Wire.begin(SDA, SCL);

  Serial.begin(115200);

  if (!vl53.begin(0x29, &Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53.vl_status);
    while (1){
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    }
     delay(10);
  }
  Serial.println(F("VL53L1X sensor OK!"));

  Serial.print(F("Sensor ID: 0x"));
  Serial.println(vl53.sensorID(), HEX);

  if (!vl53.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53.vl_status);
    while (1){
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    }
    delay(10);
  }
  Serial.println(F("Ranging started"));
  offset = 0;
  t_offset = 0;
  // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
  vl53.setTimingBudget(50);
  //Serial.print(F("Timing budget (ms): "));
  //Serial.println(vl53.getTimingBudget());

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //phyphox setup

  PhyphoxBLE::start("Laser-Distanz");
  // PhyphoxBLE::configHandler = &receivedData;
  PhyphoxBLE::experimentEventHandler = &newExperimentEvent;
  PhyphoxBLE::printXML(&Serial);
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


  firstGraph.setChannel(1, 2);

  PhyphoxBleExperiment::Graph secondGraph;  //Create graph which will plot random numbers over time
  secondGraph.setLabel("v(t)");
  secondGraph.setUnitX("s");
  secondGraph.setUnitY("cm/s");
  secondGraph.setLabelX("Zeit");
  secondGraph.setLabelY("Geschwindigkeit");
  secondGraph.setColor("4a8bad");


  // secondGraph.setStyle("dots");


  secondGraph.setChannel(1, 3);

  PhyphoxBleExperiment::Value dist;  //Creates a value-box.
  dist.setLabel("s");                //Sets the label
  dist.setPrecision(2);              //The amount of digits shown after the decimal point.
  dist.setUnit("cm");                //The physical unit associated with the displayed value.
  dist.setColor("FFFFFF");           //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  dist.setChannel(2);
  dist.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value vel;  //Creates a value-box.
  vel.setLabel("v");                //Sets the label
  vel.setPrecision(0);              //The amount of digits shown after the decimal point.
  vel.setUnit("cm/s");              //The physical unit associated with the displayed value.
  vel.setColor("FFFFFF");           //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  vel.setChannel(3);
  vel.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value bat;  //Creates a value-box.
  bat.setLabel("Batterie");         //Sets the label
  bat.setPrecision(0);              //The amount of digits shown after the decimal point.
  bat.setUnit("");                  //The physical unit associated with the displayed value.
  bat.setColor("FFFFFF");           //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  bat.setChannel(4);
  bat.setXMLAttribute("size=\"1\"");

  //Edit
  // PhyphoxBleExperiment::Edit myEdit;
  // myEdit.setLabel("Abtastrate");
  // myEdit.setUnit("ms");
  // myEdit.setSigned(false);
  // myEdit.setDecimal(false);
  // myEdit.setChannel(1);
  // myEdit.setXMLAttribute("min=\"20\"");

  /* Assign Channels, so which data is plotted on x or y axis 
   *  first parameter represents x-axis, second y-axis
   *  Channel 0 means a timestamp is created after the BLE package arrives in phyphox
   *  Channel 1 to N corresponding to the N-parameter which is written in server.write()
   */
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
  firstView.addElement(bat);

  secondView.addElement(dist);

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


  offset = 0;
  t_offset = 0;
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}


void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (!buttonState && stopped) {
    Serial.println("press");
    //t = 0.001 * (float)millis();
    //t_offset =  0.001 * (float)millis();
    offset = vl53.distance() / 10.00;
    pressed = 1;
    delay(200);
  }
  float batValue = analogRead(READBAT_PIN);

  if (vl53.dataReady() && (!stopped || !synced)) {
    //digitalWrite(LED_BUILTIN, LOW);
    distance = vl53.distance() / 10.00 - offset;
    float t = 0.001 * (float)millis() - t_offset;

    // if (!buttonState) {
    //   Serial.println("press");
    //   distance = vl53.distance() / 10.00;

    //   t = 0.001 * (float)millis();
    //   t_offset = t;
    //   offset = distance;
    // }

    d1 = d2;
    d2 = d3;
    d3 = d4;
    d4 = d5;
    t1 = t2;
    t2 = t3;
    t3 = t4;
    t4 = t5;
    d5 = distance;
    t5 = t;


    velocity = ((d5 - d4) / (t5 - t4) + (d4 - d3) / (t4 - t3) + (d3 - d2) / (t3 - t2) + (d2 - d1) / (t2 - t1)) / 4;

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
    Serial.print(velocity);
    Serial.print(",");
    Serial.print("battery");
    Serial.print(",");
    Serial.println(batValue,0);

    PhyphoxBLE::write(t, distance, velocity,batValue);

    //delay(intervall);
    vl53.clearInterrupt();
  }else{ 
    //digitalWrite(LED_BUILTIN, HIGH);
    }
}
void newExperimentEvent(){
  if(PhyphoxBLE::eventType==1){
    if(pressed){
      t_offset =  0.001 * (float)millis();
      pressed = 0;1
    }
    Serial.println("Start");
    stopped = 0;

  }
  if(PhyphoxBLE::eventType == 0 || PhyphoxBLE::eventType == 2){
    stopped = 1;
    Serial.println("Pause");
  }
  if(PhyphoxBLE::eventType==255){
    synced  = 1;
  }
}

// void receivedData() {  // get data from PhyPhox app
//   float readInput;
//   PhyphoxBLE::read(readInput);
//   intervall = readInput;
// }
