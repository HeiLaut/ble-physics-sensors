#include <phyphoxBle.h> 
#include <NewPing.h>
#include <Button.h>

#define TRIGGER_PIN 17
#define ECHO_PIN 16
#define MAX_DISTANCE 200

float duration, distance, d_old, velocity, offset,t_offset;
int timer;
Button offset_button(2);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

void setup()
{
   //Button initialisieren
   offset_button.begin();

  //phyphox setup
   
   PhyphoxBLE::start("ESP32 - Distanz");

   //Experiment
   PhyphoxBleExperiment entfernung;   //generate experiment on Arduino which plot random values

  entfernung.setTitle("Entfernungsmesser");
  entfernung.setCategory("Arduino Experiments");
  entfernung.setDescription("Die Entfernung eines Objekts zum Sensor wird in Zeitabhängigkeit gemessen");

   //View
   PhyphoxBleExperiment::View firstView;
   firstView.setLabel("FirstView"); //Create a "view"

   //Graph
   PhyphoxBleExperiment::Graph firstGraph;      //Create graph which will p lot random numbers over time     
   firstGraph.setLabel("s(t)");
   firstGraph.setUnitX("s");
   firstGraph.setUnitY("cm");
   firstGraph.setLabelX("Zeit");
   firstGraph.setLabelY("Entfernung");
   
   

   /* Assign Channels, so which data is plotted on x or y axis 
   *  first parameter represents x-axis, second y-axis
   *  Channel 0 means a timestamp is created after the BLE package arrives in phyphox
   *  Channel 1 to N corresponding to the N-parameter which is written in server.write()
   */

   firstGraph.setChannel(1,2);
   
  PhyphoxBleExperiment::Graph secondGraph;      //Create graph which will plot random numbers over time     
   secondGraph.setLabel("v(t)");
   secondGraph.setUnitX("s");
   secondGraph.setUnitY("cm/s");
   secondGraph.setLabelX("Zeit");
   secondGraph.setLabelY("Geschwindigkeit");
   secondGraph.setStyle("dots");
 

   secondGraph.setChannel(1,3);


   firstView.addElement(firstGraph);            //attach graph to view
  entfernung.addView(firstView);         //Attach view to experiment
   firstView.addElement(secondGraph);            //attach second graph to view

   PhyphoxBLE::addExperiment(entfernung);      //Attach experiment to server
   Serial.begin(9600);
   
   d_old = 0;
   timer = 0;
   offset = 0;
   t_offset = 0;
   //Serial.print("Entfernung in cm");
   //Serial.print(",     ");
   //Serial.println("Geschwindigkeit in cm/s");

}


void loop()
{
   float t = 0.001 * (float)millis()-t_offset; 
   
  if (offset_button.pressed()){
    //Serial.println("pressed");
    distance = (duration / 2) * 0.0343;
    t = 0.001 * (float)millis();
    t_offset = t;
    offset = distance;
  }
    
  timer += 1;
  if(timer == 100){
    timer =0;
  }
  
  duration = sonar.ping();
  distance = (duration / 2) * 0.0343-offset;
  
  if(timer%5 ==0){
      velocity = (distance-d_old)/0.05;
      d_old = distance;
  }
  // unsigned int distance = sonar.ping_cm();
  Serial.print(t);
  Serial.print(",     ");
  Serial.println(distance);
  
   PhyphoxBLE::write(t, distance, velocity);   
   
  delay(10);

}
