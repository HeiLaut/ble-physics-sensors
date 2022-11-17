struct Signal {
  const uint8_t PIN;
  uint32_t n;
  bool high;
  unsigned int t0; //rising time
  unsigned int t1; //rising time
  unsigned int dt; //darkening time
};

int dt = 5;
float ds = 0;
float s = 0;
float v = 0;

Signal signal1 = {17, 0, false, 0 ,0, 0};
//variables to keep track of the timing of recent interrupts

void isr() {
  if(signal1.high){
    signal1.t1 = millis()*1000;
    signal1.high = false;
  }else{
    signal1.t0 = millis()*1000;
    signal1.high = true;
  }
}

void setup() {
  //initialise_phyphox();

  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(signal1.PIN), isr, CHANGE);


}

void loop() {
  float t = 0.001 * (float)millis();
  delay(dt);
  if(signal1.t0>signal1.t1){
    signal1.dt = signal1.t0 - signal1.t1;
  }else{
    signal1.dt = signal1.t1 - signal1.t0;
  }
  
 // PhyphoxBLE::write(t, s, v);
  Serial.print("t,");Serial.print(t,3);
  Serial.print(",t1,");Serial.print(signal1.t0*0.000001,3);
  Serial.print(",t2,");Serial.print(signal1.t1*0.000001,3); 
  Serial.print(",dt,"); Serial.println(signal1.dt*0.000001,3);
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