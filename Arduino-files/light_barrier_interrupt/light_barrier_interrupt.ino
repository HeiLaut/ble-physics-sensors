// todo- Für Speichenrad evtl. Verzicht auf Variabel ddt
//ddt kann ganz aus code verschwinden!
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


int numreadings = 5;
float readings[5]={0,0,0,0,0};
float times[5]={0,0,0,0,0};


const int dt = 10;
//time between signal 1 and two
unsigned int dt_1_2 = 0;

unsigned int t_temp = 0;
int n = 0;
float s = 0;
//22,21 bei lolin32, 36,39 bei d1 mini
//lolin lite : 17/21 oder 17/16

Signal signal1 = {22, 0, false, 0 ,0, 0,0};
Signal signal2 = {21, 0, false, 0, 0, 0,0};

//variables to keep track of the timing of recent interrupts

void isr1() {
  n++;
  if(signal1.high){
    signal1.t1 = (int)millis();
      
    //calculating time between two rising signals of signal 1
    //signal1.ddt = signal1.t0 - t_temp;
  //  t_temp = signal1.t0;

    signal1.high = false;
  }else{
    signal1.t0 = (int)millis();
    signal1.high = true;
  }
}

void isr2() {
  if(signal2.high){
    signal2.t1 = (int)millis();
    signal2.high = false;
  }else{
    signal2.t0 = (int)millis();
    signal2.high = true;
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
  attachInterrupt(digitalPinToInterrupt(signal2.PIN), isr2, CHANGE);


}

void loop() {
  float t = 0.001 * (float)millis();
  //calculating darkening time using integers
  if(signal1.t0>signal1.t1){
    signal1.dt = signal1.t0 - signal1.t1;
  }else{
    signal1.dt = signal1.t1 - signal1.t0;
  }

  if(signal2.t0>signal2.t1){
    signal2.dt = signal2.t0 - signal2.t1;
  }else{
    signal2.dt = signal2.t1 - signal2.t0;
  }
  
  //calculating time between falling singal 2 and signal 1
  if(signal2.t1>signal1.t1){
    dt_1_2 = signal2.t1 - signal1.t1;
  }else{
    dt_1_2 = signal1.t1 - signal2.t1;
  }

  //calculating distance of a wheel with the diameter 49.5 mm with 15 gaps
  s = n*0.0038;//5068;

  for ( int i = 1; i < numreadings; ++i ) {
      readings[i-1] = readings[i];
      times[i-1] = times[i];
   }
  readings[numreadings-1]=s;
  times[numreadings-1]=t;

  float v = stats.slope(times,readings,numreadings);

    
  float values[6] = {t, signal1.high, s , dt_1_2*0.001, signal1.dt*0.001, signal2.dt*0.001};
  PhyphoxBLE::write(&values[0], 6);  

 // PhyphoxBLE::write(t, s, v);
  Serial.print("t,");Serial.print(t,3);
  Serial.print(",t1,");Serial.print(signal1.t0*0.001,3);
  Serial.print(",t2,");Serial.print(signal1.t1*0.001,3); 
  Serial.print(",dt,"); Serial.print(signal1.dt*0.001,3);
  Serial.print(",dt2,");Serial.print(signal2.dt*0.001,3);
  Serial.print(",s,");Serial.print(s,3);
  Serial.print(",v,");Serial.print(v,3);
  Serial.print(",d1_2,");Serial.println(dt_1_2*0.001,3);

  delay(dt);

}

void generateExperiment(void * parameter) {
  PhyphoxBleExperiment lightBarrier;


  lightBarrier.setTitle("Lichtschranke");
  lightBarrier.setCategory("Sensor-Boxen");
  lightBarrier.numberOfChannels = 6;

  PhyphoxBleExperiment::View graph;
  graph.setLabel("Graph");

  PhyphoxBleExperiment::View simple;
  simple.setLabel("Einfach");

  PhyphoxBleExperiment::View dual;
  dual.setLabel("2 Signale");

  PhyphoxBleExperiment::View wheel;
  wheel.setLabel("Speichenrad");

  PhyphoxBleExperiment::Graph sigGraph;
  sigGraph.setLabel("Signal");
  sigGraph.setUnitY("");
  sigGraph.setUnitX("s");
  sigGraph.setLabelX("Zeit t");
  sigGraph.setLabelY("");
  sigGraph.setColor("FFCC5C");
  sigGraph.setChannel(1, 2);

  PhyphoxBleExperiment::Graph distGraph;
  distGraph.setLabel("Signal");
  distGraph.setUnitY("m");
  distGraph.setUnitX("s");
  distGraph.setLabelX("Zeit t");
  distGraph.setLabelY("Weg s");
  distGraph.setColor("FFCC5C");
  distGraph.setChannel(1, 3);

  PhyphoxBleExperiment::Graph darkGraph;
  darkGraph.setLabel("Signal");
  darkGraph.setUnitY("s");
  darkGraph.setUnitX("s");
  darkGraph.setLabelX("Zeit t");
  darkGraph.setLabelY("Verdunklungszeit");
  darkGraph.setColor("FFCC5C");
  darkGraph.setChannel(1, 5);

  PhyphoxBleExperiment::Value dist;
  dist.setLabel("Diszanz s =");
  dist.setPrecision(3);
  dist.setUnit("m");
  dist.setColor("FFCC5C");
  dist.setChannel(3);
  dist.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value deltaTR;
  deltaTR.setLabel("Signallaufzeit L1 L2 R t =");
  deltaTR.setPrecision(3);
  deltaTR.setUnit("s");
  deltaTR.setColor("FFCC5C");
  deltaTR.setChannel(4);
  deltaTR.setXMLAttribute("size=\"2\"");

      // PhyphoxBleExperiment::Value ddeltaA;
      // ddeltaA.setLabel("Signallaufzeit L1 L1; t = ");
      // ddeltaA.setPrecision(3);
      // ddeltaA.setUnit("s");
      // ddeltaA.setColor("FFCC5C");
      // ddeltaA.setChannel(3);
      // ddeltaA.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value deltaA;
  deltaA.setLabel("Verdunklungszeit A t =");
  deltaA.setPrecision(3);
  deltaA.setUnit("s");
  deltaA.setColor("FFCC5C");
  deltaA.setChannel(5);
  deltaA.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value deltaB;
  deltaB.setLabel("Verdunklungszeit B t = ");
  deltaB.setPrecision(3);
  deltaB.setUnit("s");
  deltaB.setColor("FFCC5C");
  deltaB.setChannel(6);
  deltaB.setXMLAttribute("size=\"2\"");


  simple.addElement(deltaA);
  simple.addElement(darkGraph);
  //simple.addElement(ddeltaA);
  graph.addElement(sigGraph);
  
  
  wheel.addElement(distGraph);
  wheel.addElement(dist);

  dual.addElement(deltaTR);
  dual.addElement(deltaA);
  dual.addElement(deltaB);
  //simple.addElement(deltaTF);
  
  //graph.addElement(sigGraph2);
  //graph.addElement(timGraph);
  lightBarrier.addView(graph);
  lightBarrier.addView(simple);
  lightBarrier.addView(dual);
  lightBarrier.addView(wheel);

  PhyphoxBLE::addExperiment(lightBarrier);

  vTaskDelete(NULL);


}
