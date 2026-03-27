#include <phyphoxBle.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#define SIGNAL_PIN 5

//#define SCREEN_WIDTH 128  // OLED width,  in pixels
//#define SCREEN_HEIGHT 64 


//Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


int n = 0;
int t1 = 0;
int t2 = 0;
int timeArray[3] = {0,0,0};
int n_puffer = 0; //puffer to check, if a change happend.
float t_offset = 0;
float verdT = 0;
float laufT = 0;
float pendelT = 0;

bool cleared = 0;

void isr1() {
  n++;
  if(digitalRead(SIGNAL_PIN)){
    t1=(int)millis();
  }else{
    t2=(int)millis();
  }
}


void setup() {
  //Wire.begin(14, 27);
//  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//    Serial.println(F("failed to start SSD1306 OLED"));
//    while (1)
//      ;
//    }
   pinMode(LED_BUILTIN, OUTPUT);
  PhyphoxBLE::start("IR-Sensor");
  PhyphoxBLE::experimentEventHandler = &newExperimentEvent; 
  generateExperiment();
  
  Serial.begin(115200);
  pinMode(SIGNAL_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN), isr1, CHANGE);

}

void loop() {

// Gets the current runtime in seconds
  float t = 0.001 * (float)millis() - t_offset;

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

float freq = 1/laufT;

if(n_puffer != n){
  float n_f = n/2;
  PhyphoxBLE::write(t,laufT,freq,n_f);  
  Serial.print("t,");Serial.print(t,3);
  Serial.print(",Laufzeit,");Serial.print(laufT,3);
  Serial.print(",Frequenz,");Serial.print(freq,3);
  Serial.print(",n,");Serial.println(n);

//  oled.clearDisplay();
//  oled.setTextSize(3);
//  oled.setTextColor(WHITE);                            // set text color
//  oled.setCursor(0, 0);
//  oled.print(freq,3);oled.println(" Hz");
//  oled.setCursor(0, 32);
//  oled.print(verdT,3);oled.println(" s");  // set text
//  oled.display();                                      // display on OLED


}
  n_puffer = n;
}

void generateExperiment() {
  PhyphoxBleExperiment lightBarrier;

  lightBarrier.setTitle("IR-Sensor");
  lightBarrier.setCategory("Sensor-Boxen");
  lightBarrier.numberOfChannels = 6;
  lightBarrier.setDescription("IR-Reflexion");

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
  nGraph.setChannel(1,4);

  PhyphoxBleExperiment::Value laufz;
  laufz.setLabel("Signallaufzeit t =");
  laufz.setPrecision(3);
  laufz.setUnit("s");
  laufz.setColor("FFCC5C");
  laufz.setChannel(2);
  laufz.setXMLAttribute("size=\"2\"");
 
  PhyphoxBleExperiment::Value freq;
  freq.setLabel("Frequenz f =");
  freq.setPrecision(2);
  freq.setUnit("Hz");
  freq.setColor("FFCC5C");
  freq.setChannel(3);
  freq.setXMLAttribute("size=\"2\"");
  
  PhyphoxBleExperiment::Value n_;
  n_.setLabel("Anzahl n =");
  n_.setPrecision(0);
  n_.setUnit("");
  n_.setColor("FFCC5C");
  n_.setChannel(4);
  n_.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Graph periodGraph; // Graph for period over 
  periodGraph.setLabel("Laufzeiten");
  periodGraph.setUnitY("s");
  periodGraph.setUnitX("");
  periodGraph.setLabelX("n");
  periodGraph.setLabelY("t");
  periodGraph.setColor("76a5af");

  periodGraph.setStyle(STYLE_VBARS);
  periodGraph.setChannel(4, 2);

  //simple.addElement(verd);
  simple.addElement(laufz);

  graph.addElement(nGraph);
  graph.addElement(n_);
  
  //multi.addElement(schwingd);
  simple.addElement(freq);
  simple.addElement(periodGraph);

  lightBarrier.addView(simple);
  lightBarrier.addView(graph);

  PhyphoxBLE::addExperiment(lightBarrier);
}

void newExperimentEvent(){
  if(PhyphoxBLE::eventType==1){
    Serial.println("Start");
    if(cleared){
     t_offset =  0.001 * (float)millis();
    }
    cleared = 0;
    }
  
  if(PhyphoxBLE::eventType==2){
    Serial.println("Clear");
    cleared = 1;
    n = 0;
  }
}
