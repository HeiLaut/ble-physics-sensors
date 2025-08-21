#include <phyphoxBle.h> 

#define SignalPIN  17
#define SignalPIN2 16
#define Diameter 35 //in mm
#define Interruptions 20 //per Rotation
//puffer for difference in distance
float s2 = 0;

//define a great value for n so it doesnt get negative, subtractet later in the loop ; n must be integer to be handled int the interrupt function
int n = 20000;
int n1 = 0;

//rotationspeed
float rs = 0;
//loop delay
int dt = 100;
//timepuffer
unsigned long timeOld;

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
  //calculate distance in cm
  float s = (n-20000)*PI*Diameter/(Interruptions*10);
  s2 = s;

  //calculate rotation speed in rps
  rs = (n-n1)/((millis()-timeOld)*0.001);
  n1=n;
  timeOld=millis();

  //calculate velocity in cm/s
  float v = rs*PI*Diameter/(Interruptions*10);



  Serial.print("t(s),");Serial.print(t);
  Serial.print(", s(cm),");Serial.print(s,1);
  Serial.print(", n(1/s),");Serial.print(rs,1);
  //Serial.print(digitalRead(SignalPIN));
 // Serial.println(digitalRead(SignalPIN2));
  Serial.print(", v(cm/s),");Serial.println(v,1);

  PhyphoxBLE::write(t, s, v,rs);   

  delay(dt);


  }
void phyphox_init(){
  PhyphoxBLE::start("Speichenrad");
  PhyphoxBleExperiment wheel; 

  wheel.setTitle("Speichenrad");
  wheel.setCategory("Sensor-Boxen");
  wheel.setDescription("Drehzahl, Geschwindigkeit, Weg");


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

  PhyphoxBleExperiment::Value rotV;
  rotV.setLabel("n");                  //Sets the label
  rotV.setPrecision(1);                     //The amount of digits shown after the decimal point.
  rotV.setUnit("1/s");                        //The physical unit associated with the displayed value.
  rotV.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  rotV.setChannel(4);
  rotV.setXMLAttribute("size=\"2\"");

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
  secondView.addElement(rotV);


  wheel.addView(firstView);               //Attach view to experiment
  wheel.addView(secondView); 
  
  mySet.addElement(exTime);
  mySet.addElement(exT1);
  mySet.addElement(exT2);
  wheel.addExportSet(mySet);  
  PhyphoxBLE::addExperiment(wheel);

}