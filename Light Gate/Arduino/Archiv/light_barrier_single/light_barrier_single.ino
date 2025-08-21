#include <phyphoxBle.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128  // OLED width,  in pixels
#define SCREEN_HEIGHT 64  // OLED height, in pixels
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


struct Signal {
  const uint8_t PIN;
  uint32_t n;
  bool high;
  unsigned int t0; //rising time
  unsigned int t1; //rising time
  unsigned int dt; //darkening time
  unsigned int ddt; //time between two rising signals
};

float spacing = 5;

const int dt = 10;

unsigned int t_temp = 0;
int n = 0;
float s = 0;
//22,21 bei lolin32, 36,39 bei d1 mini
//lolin lite : 17/21 oder 17/16

int lastrising = 0;

Signal signal1 = {22, 0, false, 0 ,0, 0,0};

float lastmeasure
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
  attachInterrupt(digitalPinToInterrupt(signal1.PIN), isr1, CHANGE);
  Wire.begin(16,17);
}

void loop() {
  float t = 0.001 * (float)millis();
  if(signal1.t0 != lastrising){
  signal1.ddt = signal1.t0 - lastrising;
  }
  lastrising = signal1.t0;
  
//prevent the integer values to get negative
  if(signal1.t0>signal1.t1){
    signal1.dt = signal1.t0 - signal1.t1;
  }else{
    signal1.dt = signal1.t1 - signal1.t0;
  }

  //calculating distance 
  s = n*spacing*0.01;


  float values[5] = {t, signal1.high, s , signal1.ddt*0.001, signal1.dt*0.001};
  PhyphoxBLE::write(&values[0], 5);  

  Serial.print("t,");Serial.print(t,3);
  Serial.print(",t1,");Serial.print(signal1.t0*0.001,3);
  Serial.print(",dt,"); Serial.print(signal1.dt*0.001,3);
  Serial.print(",s,");Serial.print(s,3);
  Serial.print(",ddt,");Serial.println(signal1.ddt*0.001,3);
  delay(dt);

  if (change) {
    //static unsigned long last_interrupt_time = 0;
    //unsigned long interrupt_time = millis();
     // wait two seconds for initializing
    oled.clearDisplay();  // clear display

    oled.setTextSize(3);                                 // set text size
    oled.setTextColor(WHITE);                            // set text color
    
    oled.setCursor(0, 0);
    oled.print(signal1.t0*0.001,3);oled.println(" s");  // set text

                              
    oled.setCursor(0, 32);
      oled.print(signal1.dt*0.001,3);oled.println(" s");  // set text
    oled.display();                                      // display on OLED
  }
}

}
void receivedData() {           // get data from PhyPhox app
  float readInput;
  PhyphoxBLE::read(readInput);
   if(readInput>0){
      spacing = readInput;
    }
}

void generateExperiment(void * parameter) {
  PhyphoxBleExperiment lightBarrier;


  lightBarrier.setTitle("Einzel-Lichtschranke");
  lightBarrier.setCategory("Sensor-Boxen");
  lightBarrier.numberOfChannels = 5;
  lightBarrier.setDescription("Lichtschranke zur Zeit und Wegmessung");
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

  PhyphoxBleExperiment::Value ddt;
  ddt.setLabel("Signallaufzeit t =");
  ddt.setPrecision(3);
  ddt.setUnit("s");
  ddt.setColor("FFCC5C");
  ddt.setChannel(4);
  ddt.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Edit myEdit;
  myEdit.setLabel("Gitterabstand");
  myEdit.setUnit("cm");
  myEdit.setSigned(false);
  myEdit.setDecimal(true);
  myEdit.setChannel(1);
  myEdit.setXMLAttribute("min=\"1\"");



  PhyphoxBleExperiment::Value deltaA;
  deltaA.setLabel("Verdunklungszeit t =");
  deltaA.setPrecision(3);
  deltaA.setUnit("s");
  deltaA.setColor("FFCC5C");
  deltaA.setChannel(5);
  deltaA.setXMLAttribute("size=\"2\"");


  simple.addElement(deltaA);
  simple.addElement(ddt);

  graph.addElement(sigGraph);
 // graph.addElement(darkGraph);

  
  multi.addElement(distGraph);
  multi.addElement(dist);
 // multi.addElement(velGraph);
//  multi.addElement(vel);
  multi.addElement(myEdit);

  lightBarrier.addView(graph);
  lightBarrier.addView(simple);
  lightBarrier.addView(multi);

  PhyphoxBLE::addExperiment(lightBarrier);

  vTaskDelete(NULL);
}
