#include <HX711_ADC.h>
#include <phyphoxBle.h>
#include <Button.h>

HX711_ADC LoadCell(4, 5); //LoadCell(DT,SCK)
HX711_ADC LoadCell2(33,14);

#define BUTTON_PIN 2 

int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin

int touchValue;
int counter = 0;



void setup() {
  //Turn on the internal LED on lolin 32
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW);
  
  PhyphoxBLE::start("Kraft2");
    
  PhyphoxBleExperiment kraft; 
  //
  kraft.setTitle("Kraftsensor2");
  kraft.setCategory("Sensor-Boxen");
  
 //View
  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Graph"); //Create a "view"
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Einfach"); //Create a "view"
  PhyphoxBleExperiment::View waage;
  waage.setLabel("Waage");
  PhyphoxBleExperiment::View dual;
  dual.setLabel("Zwei Sensoren");
  

  //
  PhyphoxBleExperiment::Graph firstGraph;      //Create graph which will p lot random numbers over time     
  firstGraph.setLabel("Kraft");
  firstGraph.setUnitX("s");
  firstGraph.setUnitY("N");
  firstGraph.setLabelX("Zeit t");
  firstGraph.setLabelY("Kraft F");

  firstGraph.setChannel(1,2);

  PhyphoxBleExperiment::Graph secondGraph;      //Create graph which will p lot random numbers over time     
  secondGraph.setLabel("Kraft");
  secondGraph.setUnitX("s");
  secondGraph.setUnitY("N");
  secondGraph.setLabelX("Zeit t");
  secondGraph.setLabelY("Kraft F_2");

  secondGraph.setChannel(1,4);

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

  //Value
  PhyphoxBleExperiment::Value force2;         //Creates a value-box.
  force2.setLabel("F_2");                  //Sets the label
  force2.setPrecision(4);                     //The amount of digits shown after the decimal point.
  force2.setUnit("N");                        //The physical unit associated with the displayed value.
  force2.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  force2.setChannel(4);
  force2.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value mass2;         //Creates a value-box.
  mass2.setLabel("m_2");                  //Sets the label
  mass2.setPrecision(1);                     //The amount of digits shown after the decimal point.
  mass2.setUnit("g");                        //The physical unit associated with the displayed value.
  mass2.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  mass2.setChannel(5);
  mass2.setXMLAttribute("size=\"2\"");


  firstView.addElement(force);
  firstView.addElement(firstGraph); 
  secondView.addElement(force);
  waage.addElement(mass);
  dual.addElement(firstGraph);
  dual.addElement(force);
  dual.addElement(secondGraph);
  dual.addElement(force2);
  kraft.addView(firstView);
  kraft.addView(secondView);
  kraft.addView(waage);
  kraft.addView(dual);
  
  PhyphoxBLE::addExperiment(kraft);
   
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(742.73); 

  LoadCell2.begin(); // start connection to HX711
  LoadCell2.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell2.setCalFactor(1019.34); 
  
  
  Serial.begin(115200); 
  pinMode(BUTTON_PIN, INPUT_PULLUP);

}



void loop() {
  //Serial.print(touchValue);
  currentState = digitalRead(BUTTON_PIN);
   
   //touchValue = touchRead(2);

  //if(touchValue<20){
  //  counter+=1;
 // }else{
 //   counter=0;
 // }

  
  if ((lastState == HIGH && currentState == LOW)||counter>5)
    {LoadCell.tareNoDelay();
    LoadCell2.tareNoDelay();}
  else if (lastState == LOW && currentState == HIGH)
  // save the the last state
  lastState = currentState;
  //Serial.print(counter);
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
  PhyphoxBLE::write(t,f,m,f2,m2);

  Serial.print("t(s)");Serial.print(",");Serial.print(t);Serial.print(",");
  Serial.print("F(N)");Serial.print(",");Serial.print(f,3);Serial.print(",");
  Serial.print("m(g)");Serial.print(",");Serial.print(m,1);Serial.print(",");
  Serial.print("F2(N)");Serial.print(",");Serial.print(f2,3);Serial.print(",");
  Serial.print("m2(g)");Serial.print(",");Serial.println(m2,1);//Serial.print(touchValue);
  
  delay(50);

}
