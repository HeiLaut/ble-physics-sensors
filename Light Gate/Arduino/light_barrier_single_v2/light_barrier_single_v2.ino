#include <phyphoxBle.h>
#define SIGNAL_PIN 22

int n = 0;
int t1 = 0;
int t2 = 0;
int timeArray[3] = {0,0,0};
int n_puffer = 0; //puffer to check, if a change happend.

float verdT = 0;
float laufT = 0;
float pendelT = 0;

float radius = 0;

void isr1() {
  n++;
  if(digitalRead(SIGNAL_PIN)){
    t1=(int)millis();
  }else{
    t2=(int)millis();
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
  pinMode(SIGNAL_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN), isr1, CHANGE);

}

void loop() {

// Gets the current runtime in seconds
float t = 0.001 * (float)millis();

//gets darkening Time of the sensor
if(t2>t1){
  verdT = (t2-t1)*0.001;
}
 
// Measures the time between t1 and the last two rising timestamps
if(t1!=timeArray[2]){
  // Shift the timeArray to make room for the new timestamp
  for(int i = 0; i<2 ;i++){
    timeArray[i] = timeArray[i+1];
  }
  timeArray[2]=t1;
  // Calculate the time difference between the most recent and the previous rising timestamp
  laufT = (timeArray[2]-timeArray[1])*0.001;
  // Calculate the time difference between the most recent and the timestamp before the previous one
  pendelT = (timeArray[2]-timeArray[0])*0.001;
}

//calculates the time every second pass of a body bewtween gate
//useful for oscillations

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

  PhyphoxBleExperiment::Graph periodGraph; // Graph for period over 
  periodGraph.setLabel("Schwingungsdauer");
  periodGraph.setUnitY("s");
  periodGraph.setUnitX("");
  periodGraph.setLabelX("n");
  periodGraph.setLabelY("T");
  periodGraph.setColor("76a5af");
  periodGraph.setChannel(6, 4);

  simple.addElement(verd);
  simple.addElement(laufz);

  graph.addElement(nGraph);
  
  multi.addElement(schwingd);
  multi.addElement(freq);
  multi.addElement(periodGraph);

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
