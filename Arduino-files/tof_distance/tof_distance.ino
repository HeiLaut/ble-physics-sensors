#include <Adafruit_VL53L0X.h> 
#include <phyphoxBle.h> 
#include <QuickStats.h>

#define BUTTON_PIN 27
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
float readings[]={0,0,0,0,0};
float times[]={0,0,0,0,0};
int numreadings = 5;
QuickStats stats; //initialize an instance of this class                                                            
float duration, distance, d_old, velocity, offset,t_offset;
int timer;
float intervall = 20;


void setup() {
  Serial.begin(115200);
   //phyphox setup
   
   PhyphoxBLE::start("Laser-Distanz");  
   PhyphoxBLE::configHandler = &receivedData;     

   //Experiment
   PhyphoxBleExperiment entfernung;   //generate experiment on Arduino which plot random values

   entfernung.setTitle("Laser-Entfernungsmesser");
   entfernung.setCategory("Arduino Experiments");
   entfernung.setDescription("Die Entfernung eines Objekts zum Sensor wird in Zeitabhängigkeit gemessen");

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

   PhyphoxBLE::addExperiment(entfernung);      //Attach experiment to server
   
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  //Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
   // Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  //Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
   timer = 1;
   offset = 0;
   t_offset = 0;
   pinMode(BUTTON_PIN, INPUT_PULLUP);

}


void loop() {
  float t = 0.001 * (float)millis()-t_offset; 
  VL53L0X_RangingMeasurementData_t measure;
  int buttonState = digitalRead(BUTTON_PIN);
  distance = measure.RangeMilliMeter/10.00-offset;
  if (!buttonState){
    Serial.println("press");
    distance = measure.RangeMilliMeter/10;

    t = 0.001 * (float)millis();
    t_offset = t;
    offset = distance;
  }
  readings[timer-1]=distance;
  
  times[timer-1]=t;
  
  if(timer%numreadings ==0){
    velocity = stats.slope(times,readings,numreadings);
     timer = 0;
 }
  timer += 1;


    
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
    
  if (measure.RangeStatus == 4 || distance >150) {  // phase failures have incorrect data
    distance = 0;
  }
  Serial.print("t(s)");Serial.print(",");
  Serial.print(t);Serial.print(",");
  Serial.print("s(cm)");Serial.print(",");
  Serial.print(distance);Serial.print(",");
  Serial.print("v(cm/s)");Serial.print(",");
  Serial.println(velocity);
  PhyphoxBLE::write(t, distance, velocity);   
 
  delay(intervall);
}

void receivedData() {           // get data from PhyPhox app
  float readInput;
  PhyphoxBLE::read(readInput);
  intervall = readInput;
}
