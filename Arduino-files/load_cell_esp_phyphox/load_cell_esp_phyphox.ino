#include <HX711_ADC.h>
#include <phyphoxBle.h>
HX711_ADC LoadCell(16, 5); //LoadCell(DT,SCK)
float mw = 0;
int n = 0;

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

}

void loop() {
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

float sum(float value, float zeit){
  int x = 10;
  n+=1;
  mw+=value;
  if(n == x){
    //Serial.println(mw/x);
    float val = mw/x/1000;
    //Serial.println(val);
    PhyphoxBLE::write(val);
    mw = 0;
    n = 0;
    val = 0;
  }
}
