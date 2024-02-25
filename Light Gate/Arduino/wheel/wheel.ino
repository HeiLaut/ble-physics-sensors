#include <phyphoxBle.h> 

#define SignalPIN  16
#define SignalPIN2 17

float s2 = 0;
//define a great value for n so it doesnt get negative, subtractet later in the loop ; n must be integer to be handled int the interrupt function
int n = 20000;
int dt = 100;

void isr() {
  if(digitalRead(SignalPIN2)){
    n++;
  }else{
    n--;
  }
}

void setup() {
  pinMode(SignalPIN2, INPUT);           // set pin to input
  pinMode(SignalPIN, INPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(SignalPIN), isr, RISING);
  phyphox_init();
}

void loop() {
  float t = 0.001 * (float)millis();
  delay(dt);
  float s = (n*1.0-20000)*0.764;
  float v = (s-s2)/(dt*0.001);
  s2 = s;

  Serial.print("t(s),");Serial.print(t);Serial.print(", s(cm),");Serial.print(s,1);
  Serial.print(", v(cm/s),");Serial.println(v,1);

  PhyphoxBLE::write(t, s, v);   

  }
void phyphox_init(){
  PhyphoxBLE::start("Speichenrad");
  PhyphoxBleExperiment wheel; 

  wheel.setTitle("Speichenrad");
  wheel.setCategory("Sensor-Boxen");

  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Graph"); 
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Einfach");

  PhyphoxBleExperiment::Graph firstGraph;    
  firstGraph.setLabel("Weg");
  firstGraph.setUnitX("s");
  firstGraph.setUnitY("cm");
  firstGraph.setLabelX("Zeit");
  firstGraph.setLabelY("Weg");
  firstGraph.setChannel(1,2);

  PhyphoxBleExperiment::Graph secondGraph;    
  secondGraph.setLabel("Geschwindigkeit");
  secondGraph.setUnitX("s");
  secondGraph.setUnitY("cm/s");
  secondGraph.setLabelX("Zeit");
  secondGraph.setLabelY("Geschwindigkeit");
  secondGraph.setChannel(1,3);

  PhyphoxBleExperiment::Value s;         //Creates a value-box.
  s.setLabel("s");                  //Sets the label
  s.setPrecision(1);                     //The amount of digits shown after the decimal point.
  s.setUnit("cm");                        //The physical unit associated with the displayed value.
  s.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  s.setChannel(2);
  s.setXMLAttribute("size=\"2\"");
  
   PhyphoxBleExperiment::Value v;         //Creates a value-box.
  v.setLabel("v");                  //Sets the label
  v.setPrecision(1);                     //The amount of digits shown after the decimal point.
  v.setUnit("cm/s");                        //The physical unit associated with the displayed value.
  v.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  v.setChannel(3);
  v.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::ExportSet mySet;       //Provides exporting the data to excel etc.
  mySet.setLabel("Speichenrad");

  PhyphoxBleExperiment::ExportData exTime;
  exTime.setLabel("t(s)");
  exTime.setDatachannel(1);

  PhyphoxBleExperiment::ExportData exT1;
  exT1.setLabel("s(cm)");
  exT1.setDatachannel(2);

  PhyphoxBleExperiment::ExportData exT2;
  exT2.setLabel("v(cm/s)");
  exT2.setDatachannel(3);

  firstView.addElement(s);
  firstView.addElement(firstGraph);            //attach graph to view

  firstView.addElement(v);
  firstView.addElement(secondGraph);            //attach graph to view

  secondView.addElement(s);
  secondView.addElement(v);

  wheel.addView(firstView);               //Attach view to experiment
  wheel.addView(secondView); 
  
  mySet.addElement(exTime);
  mySet.addElement(exT1);
  mySet.addElement(exT2);
  wheel.addExportSet(mySet);  
  PhyphoxBLE::addExperiment(wheel);

}