#include <HX711_ADC.h>
#include <phyphoxBle.h>
#include <Preferences.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_PIN 2 
#define SENSOR_NAME "Kraftsensor OLED"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

HX711_ADC LoadCell(4, 5); //LoadCell(DT,SCK)

Preferences prefs;

//calibration factor (Default, wird durch gespeicherten Wert überschrieben)
//calFactor*shownMass/exactMass
float calFactor = 757.70;
volatile float calLoad = 0;
volatile bool  doCal   = false;
volatile bool  doTare  = false;

int tara = 0;
int reset = 0;
float t_offset = 0;

void setup() {
  Serial.begin(115200);

  prefs.begin("kraftsensor", false);
  calFactor = prefs.getFloat("calFactor", calFactor);

  Wire.begin(33,32);//SDA SCL
  //Turn on the internal LED on lolin 32
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW);
  
  PhyphoxBLE::start(SENSOR_NAME);
     // PhyphoxBLE::configHandler = &receivedData;
  PhyphoxBLE::experimentEventHandler = &newExperimentEvent;
  PhyphoxBLE::printXML(&Serial);

  PhyphoxBleExperiment kraft; 
  //
  kraft.setTitle("Kraftsensor");
  kraft.setCategory("Sensor-Boxen");

  PhyphoxBleExperiment::Button calBtn("Kalibrieren", 5);
  PhyphoxBleExperiment::Edit   calEdit("Referenzmasse", 6);
  PhyphoxBleExperiment::Button tareBtn("Tarieren", 4);

  calEdit.setUnit("g");
  calEdit.setDecimal(true);
  //calEdit.submitWith(calBtn);   // Button-Kanal ist durch den Konstruktor schon gesetzt
  calEdit.onChange([](float v) { calLoad = v; doCal = true; });

  tareBtn.onPress([]() { doTare = true; });
  
 //View
  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Graph"); //Create a "view"
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Einfach"); //Create a "view"
  PhyphoxBleExperiment::View waage;
  waage.setLabel("Waage");
  PhyphoxBleExperiment::View calib;
  calib.setLabel("Kalibrieren");
  

  //
  PhyphoxBleExperiment::Graph firstGraph;      //Create graph which will p lot random numbers over time     
  firstGraph.setLabel("Kraft");
  firstGraph.setUnitX("s");
  firstGraph.setUnitY("N");
  firstGraph.setLabelX("Zeit t");
  firstGraph.setLabelY("Kraft F");

  firstGraph.setChannel(1,2);

  //Value
  PhyphoxBleExperiment::Value force;         //Creates a value-box.
  force.setLabel("F");                  //Sets the label
  force.setPrecision(3);                     //The amount of digits shown after the decimal point.
  force.setUnit("N");                        //The physical unit associated with the displayed value.
  force.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  force.setChannel(2);
  force.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value mass;         //Creates a value-box.
  mass.setLabel("m");                  //Sets the label
  mass.setPrecision(1);                     //The amount of digits shown after the decimal point.
  mass.setUnit("g");                        //The physical unit associated with the displayed value.
  mass.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  mass.setChannel(3);
  mass.setXMLAttribute("size=\"2\"");


  firstView.addElement(force);
  firstView.addElement(firstGraph); 
  firstView.addElement(tareBtn);

  secondView.addElement(force);
  waage.addElement(mass);

  calib.addElement(calEdit);
  calib.addElement(calBtn);

  kraft.addView(firstView);
  kraft.addView(secondView);
  kraft.addView(waage);
  kraft.addView(calib);
  
  PhyphoxBLE::addExperiment(kraft);
   
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(calFactor); 
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  //delay(2000);
  display.clearDisplay();
  display.setRotation(2);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Start");
  display.display(); 
  delay(500);

}



void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if(!buttonState || doTare){
    doTare = false;
    Serial.println("press");
    LoadCell.tareNoDelay();
    while(tara == 0){
      LoadCell.update();
      tara = LoadCell.getTareStatus();
    };
    Serial.println("Tariert");
    tara = 0;
  }
  static boolean newDataReady = 0;
  if (LoadCell.update()) newDataReady = true;

  if (newDataReady && reset == false) {

  float t = 0.001 * ((float)millis() - t_offset);
  float incDat = LoadCell.getData();
  float m = abs(incDat);
  float f = incDat *9.81/1000;
  PhyphoxBLE::write(t,f,m);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10,0);
  display.print(f,3);
  display.setCursor(85,0);
  display.print('N');
  display.setCursor(10,15);
  display.print(m,1);
  display.setCursor(85,15);
  display.print('g');
  display.display();

  Serial.print("t(s)");Serial.print(",");Serial.print(t);Serial.print(",");
  Serial.print("F(N)");Serial.print(",");Serial.print(f,3);Serial.print(",");
  Serial.print("m(g)");Serial.print(",");Serial.println(m,1);

  if (doCal) {
    doCal = false;
    if (calLoad > 0 && incDat > 0) {
      calFactor = calFactor * (incDat / calLoad);   // beides in g
      prefs.putFloat("calFactor", calFactor);
      LoadCell.setCalFactor(calFactor);
      Serial.println("Kalibriert");
    }
  }

  newDataReady = false;
  }

}

void newExperimentEvent(){
  if(PhyphoxBLE::eventType==1 && reset){
    t_offset = (float)millis();
    reset = false;
  }
  if(PhyphoxBLE::eventType == 2){
    t_offset = (float)millis();
    reset = true;
  }
}
