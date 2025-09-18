#include <HX711_ADC.h>
#include <phyphoxBle.h>

#include <GxEPD2_3C.h>
#include <Fonts/FreeSansBold160pt7b.h>

#define EPD_SS 25
#define EPD_DC 33
#define EPD_RST 32
#define EPD_BUSY 35
#define MAX_DISPLAY_BUFFER_SIZE 800
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) /(EPD::WIDTH / 8))

GxEPD2_3C<GxEPD2_290_C90c, MAX_HEIGHT(GxEPD2_290_C90c)>
display(GxEPD2_290_C90c(EPD_SS, EPD_DC, EPD_RST, EPD_BUSY));


#define BUTTON_PIN 2 
#define BUTTON_PIN_D 27 

#define SENSOR_NAME "Kraftsensor M"
HX711_ADC LoadCell(4, 5); //LoadCell(DT,SCK)

//calibration factor for primary load cell
//calFactor*shownMass/exactMass
const float calFactor =  1674.1098;



int tara = 0;
int reset = 0;
float t_offset = 0;

void setup() {
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
  
 //View
  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Graph"); //Create a "view"
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Einfach"); //Create a "view"
  PhyphoxBleExperiment::View waage;
  waage.setLabel("Waage");
 

  //
  PhyphoxBleExperiment::Graph firstGraph;      //Create graph which will p lot random numbers over time     
  firstGraph.setLabel("Kraft");
  firstGraph.setUnitX("s");
  firstGraph.setUnitY("N");
  firstGraph.setLabelX("Zeit t");
  firstGraph.setLabelY("Kraft F");

  firstGraph.setChannel(1,2);

  PhyphoxBleExperiment::Graph secondGraph;      //Create graph which will p lot random numbers over time     
  secondGraph.setLabel("Kraft");
  secondGraph.setUnitX("s");
  secondGraph.setUnitY("N");
  secondGraph.setLabelX("Zeit t");
  secondGraph.setLabelY("Kraft F_2");

  secondGraph.setChannel(1,4);

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
  secondView.addElement(force);
  waage.addElement(mass);
  kraft.addView(firstView);
  kraft.addView(secondView);
  kraft.addView(waage);
  
  PhyphoxBLE::addExperiment(kraft);
   
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(calFactor); 

  
  
  Serial.begin(115200); 
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN_D, INPUT_PULLUP);
  display.init();


}



void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  int buttonState_D = digitalRead(BUTTON_PIN_D);

  
  if(!buttonState){
    Serial.println("press");
    LoadCell.tareNoDelay();
    while(tara == 0){
      LoadCell.update();
      tara = LoadCell.getTareStatus();
    };
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

  if(!buttonState_D){
        showNumber(abs(f));
        Serial.println("pressD_Button");
        delay(1000);
    }

  Serial.print("t(s)");Serial.print(",");Serial.print(t);Serial.print(",");
  Serial.print("F(N)");Serial.print(",");Serial.println(f,3);

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

void showNumber(float number)
{  
  char buf[8];
  //sprintf(buf, "%d", number);
  dtostrf(number, 2, 1, buf);
  display.setRotation(3);
  display.setFont(&FreeSansBold160pt7b);
  display.setTextColor(GxEPD_BLACK);

  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(20, 110);
    display.print(buf);
    display.setCursor(210, 110);
    display.print("N");

  } while (display.nextPage());
}
