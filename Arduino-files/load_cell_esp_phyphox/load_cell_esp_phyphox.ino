#include <HX711_ADC.h>
#include <phyphoxBle.h>
#include <Button.h>

HX711_ADC LoadCell(4, 5); //LoadCell(DT,SCK)
HX711_ADC LoadCell2(33,25);

#define BUTTON_PIN 2 

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
  LoadCell.setCalFactor(1061.05); 

  LoadCell2.begin(); // start connection to HX711
  LoadCell2.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell2.setCalFactor(1061.05); 
  
  
  Serial.begin(115200); 
  pinMode(BUTTON_PIN, INPUT_PULLUP);

}



void loop() {
  currentState = digitalRead(BUTTON_PIN);
  if (lastState == HIGH && currentState == LOW)
    {LoadCell.tareNoDelay();
    LoadCell2.tareNoDelay();}
  else if (lastState == LOW && currentState == HIGH)
  // save the the last state
  lastState = currentState;

  float t = 0.001 * (float)millis();
  LoadCell.update();
  LoadCell2.update();
  float incDat = LoadCell.getData();
  float m = abs(incDat);
  float f = -incDat *9.81/1000;
  float incDat2 = LoadCell2.getData();
  float m2 = abs(incDat2);
  float f2 = -incDat2 *9.81/1000;
  //Serial.print(t);
 // Serial.print(",");
  //Serial.println(f);
  PhyphoxBLE::write(t,f,m);

  Serial.print("t(s)");Serial.print(",");Serial.print(t);Serial.print(",");
  Serial.print("F(N)");Serial.print(",");Serial.print(f,3);Serial.print(",");
  Serial.print("m(g)");Serial.print(",");Serial.print(m,1);Serial.print(",");
  Serial.print("F2(N)");Serial.print(",");Serial.print(f2,3);Serial.print(",");
  Serial.print("m2(g)");Serial.print(",");Serial.println(m2,1);
  
  delay(50);

}
