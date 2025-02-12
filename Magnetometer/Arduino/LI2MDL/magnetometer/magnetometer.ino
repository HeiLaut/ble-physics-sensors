#include <phyphoxBle.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LIS2MDL lis2mdl = Adafruit_LIS2MDL(12345);
#define LIS2MDL_CLK 13
#define LIS2MDL_MISO 12
#define LIS2MDL_MOSI 11
#define LIS2MDL_CS 10

#define BUTTON_PIN 27 // GPIO27 pin connected to button

float xo = 0;
float yo = 0;
float zo = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW);
  Wire.begin(16,17);
  Serial.begin(115200);
  
  // assign input pullup resistor to button pin
  pinMode(BUTTON_PIN, INPUT_PULLUP);


  //initalising the lis2mdl sensor

  lis2mdl.enableAutoRange(true);

  /* Initialise the sensor */
  if (!lis2mdl.begin()) {  // I2C mode
  //if (! lis2mdl.begin_SPI(LIS2MDL_CS)) {  // hardware SPI mode
  //if (! lis2mdl.begin_SPI(LIS2MDL_CS, LIS2MDL_CLK, LIS2MDL_MISO, LIS2MDL_MOSI)) { // soft SPI
    /* There was a problem detecting the LIS2MDL ... check your connections */
    Serial.println("Ooops, no LIS2MDL detected ... Check your wiring!");
    while (1) delay(10);
  }

  /* Display some basic information on this sensor */
  lis2mdl.printSensorDetails();

  
  PhyphoxBLE::start("Magnetfeldsensor");
  PhyphoxBleExperiment magnet; 

  magnet.setTitle("Magnetfeldsensor");
  magnet.setCategory("Sensor-Boxen");
  magnet.setDescription("3-Achsen Magnetfeldsensor");

  //View
  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Graph"); 
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Einfach");//Create a "view"

  //Graph
  PhyphoxBleExperiment::Graph firstGraph;    
  firstGraph.setLabel("x");
  firstGraph.setUnitX("s");
  firstGraph.setUnitY("mT");
  firstGraph.setLabelX("Zeit");
  firstGraph.setLabelY("B");
  firstGraph.setChannel(1,2);

  PhyphoxBleExperiment::Graph secondGraph;    
  secondGraph.setLabel("y");
  secondGraph.setUnitX("s");
  secondGraph.setUnitY("mT");
  secondGraph.setLabelX("Zeit");
  secondGraph.setLabelY("B");
  secondGraph.setChannel(1,3);

  PhyphoxBleExperiment::Graph thirdGraph;    
  thirdGraph.setLabel("z");
  thirdGraph.setUnitX("s");
  thirdGraph.setUnitY("mT");
  thirdGraph.setLabelX("Zeit");
  thirdGraph.setLabelY("B");
  thirdGraph.setChannel(1,4);

  PhyphoxBleExperiment::Graph fourthGraph;    
  fourthGraph.setLabel("Gesamt");
  fourthGraph.setUnitX("s");
  fourthGraph.setUnitY("mT");
  fourthGraph.setLabelX("Zeit");
  fourthGraph.setLabelY("B");
  fourthGraph.setChannel(1,5);


  PhyphoxBleExperiment::Value bx;         //Creates a value-box.
  bx.setLabel("B-x");                  //Sets the label
  bx.setPrecision(3);                     //The amount of digits shown after the decimal point.
  bx.setUnit("mT");                        //The physical unit associated with the displayed value.
  bx.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  bx.setChannel(2);
  bx.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value by;         //Creates a value-box.
  by.setLabel("B-y");                  //Sets the label
  by.setPrecision(3);                     //The amount of digits shown after the decimal point.
  by.setUnit("mT");                        //The physical unit associated with the displayed value.
  by.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  by.setChannel(3);
  by.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value bz;         //Creates a value-box.
  bz.setLabel("B-z");                  //Sets the label
  bz.setPrecision(3);                     //The amount of digits shown after the decimal point.
  bz.setUnit("mT");                        //The physical unit associated with the displayed value.
  bz.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  bz.setChannel(4);
  bz.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value bg;         //Creates a value-box.
  bg.setLabel("Gesamt");                  //Sets the label
  bg.setPrecision(3);                     //The amount of digits shown after the decimal point.
  bg.setUnit("mT");                        //The physical unit associated with the displayed value.
  bg.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  bg.setChannel(5);
  bg.setXMLAttribute("size=\"2\"");

  firstView.addElement(firstGraph);            //attach graph to view
  firstView.addElement(secondGraph);            //attach graph to view
  firstView.addElement(thirdGraph);            //attach graph to view
  firstView.addElement(fourthGraph);

  secondView.addElement(bx);
  secondView.addElement(by);
  secondView.addElement(bz);
  secondView.addElement(bg);

  magnet.addView(firstView);               //Attach view to experiment
  magnet.addView(secondView); 
  
  
  PhyphoxBLE::addExperiment(magnet);

}

void loop() {
  float x, y, z;
  float t = 0.001 * (float)millis();
  int buttonState = digitalRead(BUTTON_PIN);

  sensors_event_t event;
  lis2mdl.getEvent(&event);
  // Read compass values
  // Return XYZ readings
  x = event.magnetic.x*0.001-xo;
  y = event.magnetic.y*0.001-yo;
  z = event.magnetic.z*0.001-zo;
  float g =sqrt(pow(x,2)+pow(y,2)+pow(z,2));
  Serial.print("t(s): ,");  Serial.print(t);
  Serial.print(",Bx(mT): ,");  Serial.print(x,3);
  Serial.print(",By(mT): ,");  Serial.print(y,3);
  Serial.print(",Bz(mT): ,");  Serial.print(z,3);
  Serial.print(",Bg(mT): ,");  Serial.println(g,3);

  if(buttonState==0){
    xo=x;
    yo=y;
    zo=z;
  }
  
  //Serial.println(buttonState);
  PhyphoxBLE::write(t, x, y, z, g);   

  delay(200);
}
