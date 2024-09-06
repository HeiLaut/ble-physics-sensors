#include <phyphoxBle.h>

int n = 0;
int t1 = 0;
int t2 = 0;
int t3 = 0;
int t4 = 0;
int puffer = 0;
int n_puffer = 0; //puffer to check, if a change happend.

float pendelT = 0;

bool s1 = false;
bool s2 = false;

float radius = 0;

void isr1() {
  n++;
  if(!s1 && !s2){
    t1=(int)millis();
    s1 = true;
  }else if(!s2){
    t2=(int)millis();
    s1=false;
    s2=true;
  }else if(!s1&&s2){
    t3=(int)millis();
    s1=true;
    s2=true;
  }else{
    t4=(int)millis();
    s1=0;
    s2=0;
  }
}


void setup() {
  PhyphoxBLE::start("Einzel-Lichtschranke");
  PhyphoxBLE::setMTU(48); //6 float values 6*4 = 24 bytes
  // An extra task takes care of the experiment creation
  xTaskCreate(
    generateExperiment,    // Function that should be called
    "experimentTask",   // Name of the task (for debugging)
    16000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );

  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(22), isr1, CHANGE);

}

void loop() {
//gets runtime
float t = 0.001 * (float)millis();

//calculating distance of rotation
float s = 2*radius*PI/40*n;
//gets darkening Time of the sensor
//because of capturing multiple si
float verdT = 0;
  if(!s1&&s2){verdT = (t2-t1)*0.001;}
  else{verdT = (t4-t3)*0.001;}
 
//measures time between t
float laufT = abs((float)t3-(float)t1)*0.001;

//calculates the time every second pass of a body bewtween gate
//useful for oscillations
if(puffer!=t1){
  pendelT = (float(t1)-float(puffer))*0.001;
}puffer = t1;
float pendelF = 1/pendelT;

if(n_puffer != n){


float values[6] = {t,laufT,verdT,pendelT,pendelF,n};
PhyphoxBLE::write(&values[0], 6);  

Serial.print("t,");Serial.print(t,3);
Serial.print(",Laufzeit,");Serial.print(laufT,3);
Serial.print(",Verdunklungszeit,"); Serial.print(verdT,3);
Serial.print(",Schwingungsdauer,");Serial.print(pendelT,3);
Serial.print(",Frequenz,");Serial.print(pendelF,3);
Serial.print(",n,");Serial.println(n);
}
n_puffer = n;


}

void generateExperiment(void * parameter) {
  PhyphoxBleExperiment lightBarrier;


  lightBarrier.setTitle("Einzel-Lichtschranke");
  lightBarrier.setCategory("Sensor-Boxen");
  lightBarrier.numberOfChannels = 6;
  lightBarrier.setDescription("Lichtschranke");

  PhyphoxBleExperiment::View graph;
  graph.setLabel("Vielfachmessung");

  PhyphoxBleExperiment::View simple;
  simple.setLabel("Laufzeiten");

  PhyphoxBleExperiment::View multi;
  multi.setLabel("Pendel");

  PhyphoxBleExperiment::Graph nGraph;
  nGraph.setLabel("Ereignisse");
  nGraph.setUnitY("");
  nGraph.setUnitX("s");
  nGraph.setLabelX("Zeit t");
  nGraph.setLabelY("");
  nGraph.setColor("FFCC5C");
  nGraph.setChannel(1,6);

  PhyphoxBleExperiment::Value verd;
  verd.setLabel("Verdunklungszeit t =");
  verd.setPrecision(3);
  verd.setUnit("s");
  verd.setColor("FFCC5C");
  verd.setChannel(3);
  verd.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value laufz;
  laufz.setLabel("Signallaufzeit t =");
  laufz.setPrecision(3);
  laufz.setUnit("s");
  laufz.setColor("FFCC5C");
  laufz.setChannel(2);
  laufz.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value schwingd;
  schwingd.setLabel("Schwingungsdauer T =");
  schwingd.setPrecision(3);
  schwingd.setUnit("s");
  schwingd.setColor("FFCC5C");
  schwingd.setChannel(4);
  schwingd.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value freq;
  freq.setLabel("Frequenz f =");
  freq.setPrecision(3);
  freq.setUnit("Hz");
  freq.setColor("FFCC5C");
  freq.setChannel(5);
  freq.setXMLAttribute("size=\"2\"");


  simple.addElement(verd);
  simple.addElement(laufz);

  graph.addElement(nGraph);
  
  multi.addElement(schwingd);
  multi.addElement(freq);

  lightBarrier.addView(simple);
  lightBarrier.addView(multi);
  lightBarrier.addView(graph);

  PhyphoxBLE::addExperiment(lightBarrier);

  vTaskDelete(NULL);
}

void receivedData() {           // get data from PhyPhox app
  float readInput;
  PhyphoxBLE::read(readInput);
   if(readInput>0){
      radius = readInput;
    }
}
