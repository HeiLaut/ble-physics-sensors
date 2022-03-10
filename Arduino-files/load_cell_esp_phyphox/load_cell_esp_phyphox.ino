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
  kraft.setCategory("Arduino Experiments");
  
 //View
  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Graph"); //Create a "view"
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Einfach"); //Create a "view"
  //
  PhyphoxBleExperiment::Graph firstGraph;      //Create graph which will p lot random numbers over time     
  firstGraph.setLabel("Kraft");
  firstGraph.setUnitX("s");
  firstGraph.setUnitY("N");
  firstGraph.setLabelX("Zeit");
  firstGraph.setLabelY("F");

  firstGraph.setChannel(1,2);

  //Value
  PhyphoxBleExperiment::Value force;         //Creates a value-box.
  force.setLabel("F");                  //Sets the label
  force.setPrecision(2);                     //The amount of digits shown after the decimal point.
  force.setUnit("N");                        //The physical unit associated with the displayed value.
  force.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  force.setChannel(2);
  force.setXMLAttribute("size=\"2\"");


  firstView.addElement(force);
  firstView.addElement(firstGraph); 
  secondView.addElement(force);
  kraft.addView(firstView);
  kraft.addView(secondView);
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
  float m = LoadCell.getData();
  float f = -m *9.81/1000;
  Serial.print(t);
  Serial.print(",");
  Serial.println(f);
  PhyphoxBLE::write(t,f);

  //sum(f, t);
  delay(50);

}
