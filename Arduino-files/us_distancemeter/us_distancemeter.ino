#include <phyphoxBle.h> 
#include <NewPing.h>
#include <Button.h>
#include <QuickStats.h>
#define TRIGGER_PIN 17
#define ECHO_PIN 16
#define MAX_DISTANCE 200
float readings[]={0,0,0,0,0};
float times[]={0,0,0,0,0};
int numreadings = 5;
QuickStats stats; //initialize an instance of this class
float duration, distance, d_old, velocity, offset,t_offset;
int timer;
Button offset_button(2);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

void setup()
{
   //Button initialisieren
   offset_button.begin();

  //phyphox setup
   
   PhyphoxBLE::start("Distanz");

   //Experiment
   PhyphoxBleExperiment entfernung;   //generate experiment on Arduino which plot random values

   entfernung.setTitle("Entfernungsmesser");
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

   /* Assign Channels, so which data is plotted on x or y axis 
   *  first parameter represents x-axis, second y-axis
   *  Channel 0 means a timestamp is created after the BLE package arrives in phyphox
   *  Channel 1 to N corresponding to the N-parameter which is written in server.write()
   */



   firstView.addElement(dist);
   firstView.addElement(firstGraph);            //attach graph to view
   secondView.addElement(dist);

   thirdView.addElement(secondGraph);           //attach second graph to view

   entfernung.addView(firstView);               //Attach view to experiment
   entfernung.addView(secondView);               //Attach view to experiment
   entfernung.addView(thirdView);               //Attach view to experiment

   PhyphoxBLE::addExperiment(entfernung);      //Attach experiment to server
   Serial.begin(115200);
   
   d_old = 0;
   timer = 1;
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
  
  readings[timer-1]=distance;
  
  times[timer-1]=t;
  
  if(timer%5 ==0){
    velocity = stats.slope(times,readings,numreadings);
     timer = 0;
 }
  timer += 1;
  Serial.print(t);
  Serial.print(",");
  Serial.println(distance);
  
   PhyphoxBLE::write(t, distance, velocity);   
   
  delay(10);

}
