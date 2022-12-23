#include <phyphoxBle.h>
#include <QuickStats.h>

QuickStats stats;

struct Signal {
  const uint8_t PIN;
  uint32_t n;
  bool high;
  unsigned int t0; //rising time
  unsigned int t1; //rising time
  unsigned int dt; //darkening time
  unsigned int ddt; //time between two rising signals
};

float distance = 5;

const int dt = 10;

unsigned int t_temp = 0;
int n = 0;
float s = 0;
//22,21 bei lolin32, 36,39 bei d1 mini
//lolin lite : 17/21 oder 17/16

Signal signal1 = {22, 0, false, 0 ,0, 0,0};

//variables to keep track of the timing of recent interrupts

void isr1() {
  n++;
  if(signal1.high){
    signal1.t1 = (int)millis();
    signal1.high = false;
  }else{
    signal1.t0 = (int)millis();
    signal1.high = true;
  }
}


void setup() {
  PhyphoxBLE::start("Lichtschranke");
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
  attachInterrupt(digitalPinToInterrupt(signal1.PIN), isr1, CHANGE);


}

void loop() {
  float t = 0.001 * (float)millis();

  if(signal1.t0>signal1.t1){
    signal1.dt = signal1.t0 - signal1.t1;
  }else{
    signal1.dt = signal1.t1 - signal1.t0;
  }


  //calculating distance 
  s = n*distance*0.01;//5068;

  float v = distance*0.01/(signal1.dt*0.001);
  if(v>100){
    v=0;
  }
    
  float values[5] = {t, signal1.high, s , v, signal1.dt*0.001};
  PhyphoxBLE::write(&values[0], 5);  

  Serial.print("t,");Serial.print(t,3);
  Serial.print(",t1,");Serial.print(signal1.t0*0.001,3);
  Serial.print(",dt,"); Serial.print(signal1.dt*0.001,3);
  Serial.print(",s,");Serial.print(s,3);
  Serial.print(",v,");Serial.print(v,3);
  Serial.print(",distance,");Serial.println(distance);
  delay(dt);

}
void receivedData() {           // get data from PhyPhox app
  float readInput;
  PhyphoxBLE::read(readInput);
  distance = readInput;
}

void generateExperiment(void * parameter) {
  PhyphoxBleExperiment lightBarrier;


  lightBarrier.setTitle("Lichtschranke");
  lightBarrier.setCategory("Sensor-Boxen");
  lightBarrier.numberOfChannels = 5;
  PhyphoxBLE::configHandler = &receivedData;  

  PhyphoxBleExperiment::View graph;
  graph.setLabel("Graph");

  PhyphoxBleExperiment::View simple;
  simple.setLabel("Einfach");

  PhyphoxBleExperiment::View multi;
  multi.setLabel("Mehrfachmessung");

  PhyphoxBleExperiment::Graph sigGraph;
  sigGraph.setLabel("Signal");
  sigGraph.setUnitY("");
  sigGraph.setUnitX("s");
  sigGraph.setLabelX("Zeit t");
  sigGraph.setLabelY("");
  sigGraph.setColor("FFCC5C");
  sigGraph.setChannel(1, 2);

  PhyphoxBleExperiment::Graph distGraph;
  distGraph.setLabel("Weg");
  distGraph.setUnitY("m");
  distGraph.setUnitX("s");
  distGraph.setLabelX("Zeit t");
  distGraph.setLabelY("s");
  distGraph.setColor("FFCC5C");
  distGraph.setChannel(1, 3);


  PhyphoxBleExperiment::Graph velGraph;
  velGraph.setLabel("Geschwindigkeit");
  velGraph.setUnitY("m");
  velGraph.setUnitX("m/s");
  velGraph.setLabelX("Zeit t");
  velGraph.setLabelY("v in m/s");
  velGraph.setColor("FFCC5C");
  velGraph.setChannel(1, 4);

  PhyphoxBleExperiment::Graph darkGraph;
  darkGraph.setLabel("Laufzeit");
  darkGraph.setUnitY("s");
  darkGraph.setUnitX("s");
  darkGraph.setLabelX("Zeit t");
  darkGraph.setLabelY("Laufzeit");
  darkGraph.setColor("FFCC5C");
  darkGraph.setChannel(1, 5);

  PhyphoxBleExperiment::Value dist;
  dist.setLabel("Distanz s =");
  dist.setPrecision(3);
  dist.setUnit("m");
  dist.setColor("FFCC5C");
  dist.setChannel(3);
  dist.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value vel;
  vel.setLabel("Geschwindigkeit v =");
  vel.setPrecision(3);
  vel.setUnit("m/s");
  vel.setColor("FFCC5C");
  vel.setChannel(4);
  vel.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Edit myEdit;
  myEdit.setLabel("Gitterabstand");
  myEdit.setUnit("cm");
  myEdit.setSigned(false);
  myEdit.setDecimal(true);
  myEdit.setChannel(1);
  myEdit.setXMLAttribute("min=\"1\"");



  PhyphoxBleExperiment::Value deltaA;
  deltaA.setLabel("Laufzeit t =");
  deltaA.setPrecision(3);
  deltaA.setUnit("s");
  deltaA.setColor("FFCC5C");
  deltaA.setChannel(5);
  deltaA.setXMLAttribute("size=\"2\"");


  simple.addElement(deltaA);
  simple.addElement(darkGraph);
  graph.addElement(sigGraph);
  
  
  multi.addElement(distGraph);
  multi.addElement(dist);
  multi.addElement(velGraph);
  multi.addElement(vel);
  multi.addElement(myEdit);

  lightBarrier.addView(graph);
  lightBarrier.addView(simple);
  lightBarrier.addView(multi);

  PhyphoxBLE::addExperiment(lightBarrier);

  vTaskDelete(NULL);
}
