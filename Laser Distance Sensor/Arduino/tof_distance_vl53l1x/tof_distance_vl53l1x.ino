#include <phyphoxBle.h> 
#include <QuickStats.h>
#include "Adafruit_VL53L1X.h"

#define IRQ_PIN 2
#define XSHUT_PIN 3

Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);

#define BUTTON_PIN 27

float readings[5]={0,0,0,0,0};
float times[5]={0,0,0,0,0};
int numreadings = 5;
QuickStats stats; //initialize an instance of this class                                                            
float duration, d_old, velocity, offset,distance,t_offset, t;
float intervall = 20;

bool interrupt = false;

void isr(){
  static int last_interrupt = 0;
  int interrupt_time=millis();
  if(interrupt_time-last_interrupt>100){
    interrupt = true;
  }
  last_interrupt = interrupt_time;
}


void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  int sda = 15;
  int scl = 13;
  Wire.begin(sda, scl);
  if (!vl53.begin(0x29, &Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(10);
  }
  Serial.println(F("VL53L1X sensor OK!"));

  Serial.print(F("Sensor ID: 0x"));
  Serial.println(vl53.sensorID(), HEX);

  if (!vl53.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(10);
  }
  Serial.println(F("Ranging started"));

  // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
  vl53.setTimingBudget(50);
  Serial.print(F("Timing budget (ms): "));
  Serial.println(vl53.getTimingBudget());

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), isr, RISING);

  /*
  vl.VL53L1X_SetDistanceThreshold(100, 300, 3, 1);
  vl.VL53L1X_SetInterruptPolarity(0);
  */
   //phyphox setup
   
   PhyphoxBLE::start("Laser-Distanz");  
   PhyphoxBLE::configHandler = &receivedData;     

   //Experiment
   PhyphoxBleExperiment entfernung;   //generate experiment on Arduino which plot random values

   entfernung.setTitle("Laser-Entfernungsmesser");
   entfernung.setCategory("Sensor-Boxen");
   entfernung.setDescription("IR-Laser Entfernungsmessung");

   //View
   PhyphoxBleExperiment::View firstView;
   firstView.setLabel("Graph"); 
   PhyphoxBleExperiment::View secondView;
   secondView.setLabel("Einfach");//Create a "view"
   PhyphoxBleExperiment::View thirdView;
   thirdView.setLabel("Geschwindigkeit");//Create a "view"

   

   //Graph
   PhyphoxBleExperiment::Graph firstGraph;    
   firstGraph.setLabel("s(t)");
   firstGraph.setUnitX("s");
   firstGraph.setUnitY("cm");
   firstGraph.setLabelX("Zeit");
   firstGraph.setLabelY("Entfernung");


   firstGraph.setChannel(1,2);
   
   PhyphoxBleExperiment::Graph secondGraph;      //Create graph which will plot random numbers over time     
   secondGraph.setLabel("v(t)");
   secondGraph.setUnitX("s");
   secondGraph.setUnitY("cm/s");
   secondGraph.setLabelX("Zeit");
   secondGraph.setLabelY("Geschwindigkeit");
   //secondGraph.setStyle("dots");

 
   secondGraph.setChannel(1,3);
   
   PhyphoxBleExperiment::Value dist;         //Creates a value-box.
   dist.setLabel("s");                  //Sets the label
   dist.setPrecision(2);                     //The amount of digits shown after the decimal point.
   dist.setUnit("cm");                        //The physical unit associated with the displayed value.
   dist.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
   dist.setChannel(2);
   dist.setXMLAttribute("size=\"2\"");

   PhyphoxBleExperiment::Value vel;         //Creates a value-box.
   vel.setLabel("v");                  //Sets the label
   vel.setPrecision(0);                     //The amount of digits shown after the decimal point.
   vel.setUnit("cm/s");                        //The physical unit associated with the displayed value.
   vel.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
   vel.setChannel(3);
   vel.setXMLAttribute("size=\"2\"");

       //Edit
  PhyphoxBleExperiment::Edit myEdit;
  myEdit.setLabel("Abtastrate");
  myEdit.setUnit("ms");
  myEdit.setSigned(false);
  myEdit.setDecimal(false);
  myEdit.setChannel(1);
  myEdit.setXMLAttribute("min=\"20\"");

   /* Assign Channels, so which data is plotted on x or y axis 
   *  first parameter represents x-axis, second y-axis
   *  Channel 0 means a timestamp is created after the BLE package arrives in phyphox
   *  Channel 1 to N corresponding to the N-parameter which is written in server.write()
   */
      //Export
  PhyphoxBleExperiment::ExportSet mySet;       //Provides exporting the data to excel etc.
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
   firstView.addElement(firstGraph);            //attach graph to view
   firstView.addElement(myEdit);
   
   secondView.addElement(dist);
   
   thirdView.addElement(firstGraph); 
   thirdView.addElement(vel);
   thirdView.addElement(secondGraph);           //attach second graph to view

   entfernung.addView(firstView);               //Attach view to experiment
   entfernung.addView(secondView);               //Attach view to experiment
   entfernung.addView(thirdView);               //Attach view to experiment

   mySet.addElement(exTime);
   mySet.addElement(exDist);
   mySet.addElement(exVel);
   entfernung.addExportSet(mySet);  
  
   PhyphoxBLE::addExperiment(entfernung);      //Attach experiment to server
  
   offset = 0;
   t_offset = 0;
   //pinMode(BUTTON_PIN, INPUT_PULLUP);

}

void loop() {
  
  t = 0.001*millis()-t_offset; 
  if(interrupt){
    t = t+t_offset;
    distance = distance+offset;

    offset = distance;
    t_offset = t;
    interrupt = false;
  }
  if (vl53.dataReady()) {
    // new measurement for the taking!
    distance = vl53.distance()*0.1-offset;
    if (distance == -1) {
      // something went wrong!
      distance = 0;
      return;
    }
  for ( int i = 1; i < 5; ++i ) {
      readings[i-1] = readings[i];
      times[i-1] = times[i];
   }
       
  readings[4]=distance;
  times[4]=t;

  velocity = stats.slope(times,readings,numreadings);
  Serial.print("t(s)");Serial.print(",");
  Serial.print(t);Serial.print(",");
  Serial.print("s(cm)");Serial.print(",");
  Serial.print(distance);Serial.print(",");
  Serial.print("v(cm/s)");Serial.print(",");
  Serial.println(velocity);
  PhyphoxBLE::write(t, distance, velocity);   
 

    // data is read out, time for another reading!
  vl53.clearInterrupt();
  }
}

// void loop() {
//   float t = 0.001 * (float)millis()-t_offset; 
//    if (vl53.dataReady()) {
//     // new measurement for the taking!
//     distance = vl53.distance()/10;//-ofset
//     if (distance == -1) {
//       // something went wrong!
//       distance = 0;
//       return;
//     }
//     // data is read out, time for another reading!
//     vl53.clearInterrupt();
//   }
//   int buttonState = digitalRead(BUTTON_PIN);
//   if (!buttonState){
//     Serial.println("press");
//     //distance = vl53.distance()/10.00;
//     //vl53.clearInterrupt();
//     t = 0.001 * (float)millis();
//     t_offset = t;
//     offset = distance;
//   }
//   for ( int i = 1; i < 5; ++i ) {
//       readings[i-1] = readings[i];
//       times[i-1] = times[i];
//    }
       
//   readings[4]=distance;
//   times[4]=t;
//   velocity = stats.slope(times,readings,numreadings);


    

//   Serial.print("t(s)");Serial.print(",");
//   Serial.print(t);Serial.print(",");
//   Serial.print("s(cm)");Serial.print(",");
//   Serial.print(distance);Serial.print(",");
//   Serial.print("v(cm/s)");Serial.print(",");
//   Serial.println(velocity);
//   PhyphoxBLE::write(t, distance, velocity);   
 
//   delay(intervall);

// }

void receivedData() {           // get data from PhyPhox app
  float readInput;
  PhyphoxBLE::read(readInput);
  intervall = readInput;
}
