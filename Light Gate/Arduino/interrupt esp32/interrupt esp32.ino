struct Signal {
  const uint8_t PIN;
  uint32_t n;
  bool high;
};
#include <phyphoxBle.h>
#include <QuickStats.h>
float readings[10] = {0, 0, 0, 0, 0 };
float times[10] = {0, 0, 0, 0, 0 };
int numreadings = 5;
QuickStats stats;

int dt = 5;
float ds = 0;
float s = 0;
float v = 0;

//Speichen pro rad
int n = 10;

Signal signal1 = { 17, 0, false };
//variables to keep track of the timing of recent interrupts
unsigned long signal_time = 0;
unsigned long last_signal_time = 0;

void IRAM_ATTR isr() {
  signal_time = millis();
  //if (signal_time - last_signal_time > 10) {
    signal1.n++;
    //signal1.high = true;
    //last_signal_time = signal_time;
  //}
}

void IRAM_ATTR isf(){
  
}

void setup() {
  //initialise_phyphox();

  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(signal1.PIN), isr, RISING);
  attachInterrupt(digitalPinToInterrupt(signal1.PIN), isf, FALING);

}

void loop() {
  float t = 0.001 * (float)millis();
  delay(dt);
  //calculates distance of travel - pi * diameter wheel
  if (signal1.n > 0) {
    ds = n;// 0.5 * signal1.n * 3.14 * 0.052 / n;
    v = ds/dt;
    s += ds;
  } else {
    s += 0;
    v = 0;
  };
  for (int i = 1; i < numreadings; ++i) {
    readings[i - 1] = readings[i];
    times[i - 1] = times[i];
  }

  readings[numreadings - 1] = s;
  times[numreadings - 1] = t;
  v = stats.slope(times, readings, numreadings);
  signal1.n = 0;
 // PhyphoxBLE::write(t, s, v);
  Serial.print("t,");Serial.print(t,3);
  Serial.print(",s,");Serial.print(s); 
  Serial.print(",v,"); Serial.println(v);
}

// void initialise_phyphox() {
//   PhyphoxBLE::start();  //Start the BLE server
//   PhyphoxBleExperiment speichenrad;
//   PhyphoxBleExperiment::View firstView;
//   firstView.setLabel("Graph");

//   PhyphoxBleExperiment::Graph firstGraph;

//   firstGraph.setLabel("Weg(Zeit)");
//   firstGraph.setUnitX("s");
//   firstGraph.setUnitY("m");
//   firstGraph.setLabelX("Zeit");
//   firstGraph.setLabelY("Distanz");
//   firstGraph.setChannel(1, 2);

//   PhyphoxBleExperiment::Graph secondGraph;

//   secondGraph.setLabel("Geschwindigkeit(Zeit)");
//   secondGraph.setUnitX("v");
//   secondGraph.setUnitY("m/s");
//   secondGraph.setLabelX("Zeit");
//   secondGraph.setLabelY("Geschwindigkeit");
//   secondGraph.setChannel(1, 3);

//   firstView.addElement(firstGraph);
//   firstView.addElement(secondGraph);

//   speichenrad.addView(firstView);
//   PhyphoxBLE::addExperiment(speichenrad);
// }