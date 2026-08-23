#include <phyphoxBle.h>

#define INPUT_PIN 22 //pin for slight gate signal

void receivedData();

int n = 0; //counter
int t1 = 0; //time 1
int t2 = 0; //time 2
bool sig = false; //check signal to get two times
int last_signal = 0; //puffer to calculate runtime
float radius = 0; //radius
float length = 20.0;
float run_T = 0;



void isr1() {
  n++;
  if(!sig){
    t1=(int)millis();
    sig = true;
  }else{
    t2=(int)millis();
    sig = false;
  }
}


void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), isr1, CHANGE);

  PhyphoxBLE::start("Lichtschranke-Einfach");
  PhyphoxBLE::configHandler=&receivedData;

  PhyphoxBleExperiment lightBarrier;

  lightBarrier.setTitle("Lichtschranke-Einfach");
  lightBarrier.setCategory("Sensor-Boxen");
  //lightBarrier.numberOfChannels = 6;
  lightBarrier.setDescription("Laufzeit, Geschwindigkeit, Weg (Leiter)");

  PhyphoxBleExperiment::View graphV;
  graphV.setLabel("Strecke");

  PhyphoxBleExperiment::View timeV;
  timeV.setLabel("Laufzeit");

  PhyphoxBleExperiment::View speedV;
  speedV.setLabel("Geschwindigkeit");

  PhyphoxBleExperiment::Graph distGraph;
  distGraph.setLabel("Strecke");
  distGraph.setUnitY("cm");
  distGraph.setUnitX("s");
  distGraph.setLabelX("Zeit t");
  distGraph.setLabelY("Strecke");
  distGraph.setColor("FFCC5C");
  distGraph.setChannel(1,4);

  PhyphoxBleExperiment::Value speed;
  speed.setLabel("Geschwindigkeit v =");
  speed.setPrecision(1);
  speed.setUnit("cm/s");
  speed.setColor("FFCC5C");
  speed.setChannel(3);
  speed.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value laufz;
  laufz.setLabel("Signallaufzeit t =");
  laufz.setPrecision(3);
  laufz.setUnit("s");
  laufz.setColor("FFCC5C");
  laufz.setChannel(2);
  laufz.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Edit fahne;
  fahne.setLabel("Länge Fahne = ");
  fahne.setUnit("mm");
  fahne.setSigned(false);
  fahne.setDecimal(false);
  fahne.setChannel(1);
  //fahne.setXMLAttribute("min=\"1\"");

  timeV.addElement(laufz);
  speedV.addElement(speed);
  speedV.addElement(fahne);

  graphV.addElement(distGraph);
  graphV.addElement(fahne);
  
  lightBarrier.addView(timeV);
  lightBarrier.addView(speedV);
  lightBarrier.addView(graphV);


  PhyphoxBLE::addExperiment(lightBarrier);
}

void loop() {
//gets runtime
float t = 0.001 * (float)millis();

//measure darkening time
float dark_T = (t2-t1)*0.001;

//Calculate runtime between two signals

if(last_signal != t1){
  run_T = (float(t1)-float(last_signal))*0.001;
}
last_signal = t1;

//calculate velocity
float vel = abs(length*0.1/dark_T);

//calculate length
float dist = n*length*0.1;

PhyphoxBLE::write(t, run_T, vel, dist);  

Serial.print("t,");Serial.print(t,3);
Serial.print(",Laufzeit,");Serial.print(run_T,3);
Serial.print(",Geschwindigkeit,"); Serial.print(vel,1);
Serial.print(",Strecke,");Serial.print(dist,1);
Serial.print(",n,");Serial.println(n,0);

delay(20);                                  


}



void receivedData() {           // get data from PhyPhox app
  float readInput;
  PhyphoxBLE::read(readInput);
   if(readInput>0){
      length = readInput;
    }
}
