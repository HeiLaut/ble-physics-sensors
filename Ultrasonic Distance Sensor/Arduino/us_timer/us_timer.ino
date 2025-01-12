#include <phyphoxBle.h> 
#include <NewPing.h>
#define TRIGGER_PIN 17
#define ECHO_PIN 16
#define MAX_DISTANCE 200
#define BUTTON_PIN 2

float duration = 0;


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  PhyphoxBLE::start("US-Laufzeit");

  PhyphoxBleExperiment Laufzeit;

  Laufzeit.setTitle("Ultraschall-Timer");
  Laufzeit.setCategory("Sensor-Boxen");
  Laufzeit.setDescription("Misst die Laufzeit eines Ultraschallsignals, welches von einem Hindernis reflekiert wird.");

  //View
  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Label"); //Create a "view"

  //Graph
  //  PhyphoxBleExperiment::Graph firstGraph;      //Create graph which will plot random numbers over time
  //  firstGraph.setLabel("Voltmeter");
  //  firstGraph.setUnitX("s");
  //  firstGraph.setUnitY("V");
  //  firstGraph.setLabelX("time");
  //  firstGraph.setLabelY("Voltage");

  //  firstGraph.setChannel(0, 1);

  PhyphoxBleExperiment::Value myValue;         //Creates a value-box.
  myValue.setLabel("Laufzeit");                  //Sets the label
  myValue.setPrecision(0);                     //The amount of digits shown after the decimal point.
  myValue.setUnit("us");                        //The physical unit associated with the displayed value.
  myValue.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  myValue.setChannel(1);
  myValue.setXMLAttribute("size=\"2\"");


  firstView.addElement(myValue);            //attach graph to view
  //firstView.addElement(firstGraph);            //attach graph to view

  Laufzeit.addView(firstView);                //Attach view to experiment
  PhyphoxBLE::addExperiment(Laufzeit);        //Attach experiment to server

  
    // PhyphoxBLE::start("Ultraschall-Timer");
    // PhyphoxBleExperiment Zeit;
    // Zeit.setTitle("Ultraschall-Timer");
    // Zeit.setCategory("Sensor-Boxen");
    // Zeit.setDescription("Misst die Laufzeit eines Ultraschallsignals, welches von einem Hindernis reflekiert wird.");


    // PhyphoxBleExperiment::View firstView;

    // firstView.setLabel("Laufzeit"); 

    // PhyphoxBleExperiment::Value response;        
    // response.setLabel("t");                 
    // response.setPrecision(0);                   
    // response.setUnit("Mikrosekunden");                  
    // response.setChannel(1);
    // response.setXMLAttribute("size=\"2\"");
    
    
    // firstView.addElement(response);
    // Zeit.addView(firstView); 

    // PhyphoxBLE::addExperiment(Zeit);      //Attach experiment to server
}

void loop()
{
  
  if (!digitalRead(BUTTON_PIN)){
     duration = sonar.ping();
     Serial.println(duration);
     PhyphoxBLE::write(duration);   

     delay(200);
 }

   
}
