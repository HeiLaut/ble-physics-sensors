#include <phyphoxBle.h> 

struct Signal {
  const uint8_t PIN;
  uint32_t n;
  bool high;
  unsigned int t0; 
  unsigned int t1;  
  unsigned int s0;
  unsigned int s1;
};

Signal signal1 = { 16, 0, false, 0, 0, 0, 0};
//Signal signal2 = { 17, 0, false, 0, 0 };

int dr = 0;
int n = 20000;
int dt = 20;

void isr() {
  if(digitalRead(17)){
    dr=1;
  }else{
    dr=0;
  }
  
  if(dr){
    n++;
  }else{
    n--;
  }

  signal1.t1 = signal1.t0;
  signal1.t0 = (int)millis();
  signal1.s1 = signal1.s0;
  signal1.s0 = n;
}

void setup() {
  pinMode(17, INPUT);           // set pin to input
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(signal1.PIN), isr, RISING);
  phyphox_init();
}

void loop() {
  float t = 0.001 * (float)millis();
  delay(dt);
  float v = 0;
  float s = (n*1.0-20000)*0.764;
  if(signal1.t0 != signal1.t1){
    v = (signal1.s1*1.0-signal1.s0*1.0)/(signal1.t1*0.001-signal1.t0*0.001);
  }

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