#include <HX711_ADC.h>
#include <phyphoxBle.h>
#include <Button.h>
HX711_ADC LoadCell(16, 5); //LoadCell(DT,SCK)
float mw = 0;
int n = 0;
#define BUTTON_PIN 21 
//Button offset_button(35);
int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin


void setup() {
  PhyphoxBLE::start("Kraft");
    
  PhyphoxBleExperiment kraft; 
  //
  kraft.setTitle("Kraftsensor");
  kraft.setCategory("Arduino Experiments");
  //
  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("FirstView"); //Create a "view"
  //
  PhyphoxBleExperiment::Graph firstGraph;      //Create graph which will p lot random numbers over time     
  firstGraph.setLabel("Kraft");
  firstGraph.setUnitX("s");
  firstGraph.setUnitY("mN");
  firstGraph.setLabelX("Zeit");
  firstGraph.setLabelY("F");
  //
  firstGraph.setChannel(1,2);
  
  firstView.addElement(firstGraph); 
  kraft.addView(firstView);
  PhyphoxBLE::addExperiment(kraft);
   
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(717.188); 
  Serial.println("F in mN");
  Serial.begin(9600); 
  pinMode(BUTTON_PIN, INPUT_PULLUP);

}

void loop() {
    currentState = digitalRead(BUTTON_PIN);

  if (lastState == HIGH && currentState == LOW)
    LoadCell.tareNoDelay();
  else if (lastState == LOW && currentState == HIGH)
    //Serial.println("The button is released");

  // save the the last state
  lastState = currentState;

  float t = 0.001 * (float)millis();
  LoadCell.update();
  float m = LoadCell.getData();
  float f = m * 9.81;
  Serial.print(t);
  Serial.print(",");
  Serial.println(f);
  PhyphoxBLE::write(t,f);

  //sum(f, t);
  delay(50);

}
