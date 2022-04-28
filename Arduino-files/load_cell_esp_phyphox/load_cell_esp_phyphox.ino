#include <HX711_ADC.h>
#include <phyphoxBle.h>
#include <Button.h>

HX711_ADC LoadCell(16, 5); //LoadCell(DT,SCK)
#define BUTTON_PIN 21 

int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin





void setup() {
  
  PhyphoxBLE::start("Kraft");
    
  PhyphoxBleExperiment kraft; 
  //
  kraft.setTitle("Kraftsensor");
  kraft.setCategory("Sensor-Boxen");
  
 //View
  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Graph"); //Create a "view"
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Einfach"); //Create a "view"
  PhyphoxBleExperiment::View waage;
  waage.setLabel("Waage");
  

  //
  PhyphoxBleExperiment::Graph firstGraph;      //Create graph which will p lot random numbers over time     
  firstGraph.setLabel("Kraft");
  firstGraph.setUnitX("s");
  firstGraph.setUnitY("N");
  firstGraph.setLabelX("Zeit t");
  firstGraph.setLabelY("Kraft F");

  firstGraph.setChannel(1,2);

  //Value
  PhyphoxBleExperiment::Value force;         //Creates a value-box.
  force.setLabel("F");                  //Sets the label
  force.setPrecision(4);                     //The amount of digits shown after the decimal point.
  force.setUnit("N");                        //The physical unit associated with the displayed value.
  force.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  force.setChannel(2);
  force.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value mass;         //Creates a value-box.
  mass.setLabel("m");                  //Sets the label
  mass.setPrecision(1);                     //The amount of digits shown after the decimal point.
  mass.setUnit("g");                        //The physical unit associated with the displayed value.
  mass.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  mass.setChannel(3);
  mass.setXMLAttribute("size=\"2\"");


  firstView.addElement(force);
  firstView.addElement(firstGraph); 
  secondView.addElement(force);
  waage.addElement(mass);
  kraft.addView(firstView);
  kraft.addView(secondView);
  kraft.addView(waage);
  PhyphoxBLE::addExperiment(kraft);
   
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(741.896); 
  
  Serial.println("F in ");
  Serial.begin(115200); 
  pinMode(BUTTON_PIN, INPUT_PULLUP);

}



void loop() {
  currentState = digitalRead(BUTTON_PIN);
  if (lastState == HIGH && currentState == LOW)
    LoadCell.tareNoDelay();
  else if (lastState == LOW && currentState == HIGH)
  // save the the last state
  lastState = currentState;

  float t = 0.001 * (float)millis();
  LoadCell.update();
  float m = abs(LoadCell.getData());
  float f = -m *9.81/1000;
  //Serial.print(t);
 // Serial.print(",");
  //Serial.println(f);
  PhyphoxBLE::write(t,f,m);

  delay(50);

}
