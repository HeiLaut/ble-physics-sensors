#include <phyphoxBle.h> 
#include <NewPing.h>
#include <Button.h>
#include <QuickStats.h>
#define TRIGGER_PIN 17
#define ECHO_PIN 16
#define MAX_DISTANCE 200
float readings[5]={0,0,0,0,0};
float times[5]={0,0,0,0,0};
int numreadings = 5;
QuickStats stats; //initialize an instance of this class
float duration, distance, d_old, velocity, offset,t_offset;
Button offset_button(2);
float intervall = 20;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

void setup()
{
   //Button initialisieren
   offset_button.begin();

  //phyphox setup
   
   PhyphoxBLE::start("US-Distanz");


   PhyphoxBLE::configHandler = &receivedData;

   //Experiment
   PhyphoxBleExperiment entfernung; 
   

   entfernung.setTitle("Ultraschall-Entfernungsmesser");
   entfernung.setCategory("Sensor-Boxen");
   entfernung.setDescription("Ultraschall Entfernungsmesser");
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
   dist.setPrecision(1);                     //The amount of digits shown after the decimal point.
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

   /* Assign Channels, so which data is plotted on x or y axis 
   *  first parameter represents x-axis, second y-axis
   *  Channel 0 means a timestamp is created after the BLE package arrives in phyphox
   *  Channel 1 to N corresponding to the N-parameter which is written in server.write()
   */
 //Edit
  PhyphoxBleExperiment::Edit myEdit;
  myEdit.setLabel("Abtastrate");
  myEdit.setUnit("ms");
  myEdit.setSigned(false);
  myEdit.setDecimal(false);
  myEdit.setChannel(1);
  myEdit.setXMLAttribute("min=\"20\"");



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
   Serial.begin(115200);
   
   offset = 0;
   t_offset = 0;


}


void loop()
{
   float t = 0.001 * (float)millis()-t_offset; 
   
  if (offset_button.pressed()){
    distance = round((duration / 2) * 0.0343*10)/10;
    t = 0.001 * (float)millis();
    t_offset = t;
    offset = distance;
  }
    
  
  duration = sonar.ping();
  distance = round(((duration / 2) * 0.0343-offset)*10)/10;
  
  for ( int i = 1; i < 5; ++i ) {
      readings[i-1] = readings[i];
      times[i-1] = times[i];
   }
       
  readings[4]=distance;
  times[4]=t;
  velocity = stats.slope(times,readings,numreadings);
  
  Serial.print("t(2)");Serial.print(",");Serial.print(t); Serial.print(",");
  Serial.print("s(cm)");Serial.print(",");Serial.print(distance);Serial.print(",");
  Serial.print("v(cm/s)");Serial.print(",");Serial.println(velocity);
  
   PhyphoxBLE::write(t, distance, velocity);   
   
  delay(intervall);

}

void receivedData() {           // get data from PhyPhox app
  float readInput;
  PhyphoxBLE::read(readInput);
  intervall = readInput;
}
