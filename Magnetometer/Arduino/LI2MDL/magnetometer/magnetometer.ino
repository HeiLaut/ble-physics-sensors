#include <phyphoxBle.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LIS2MDL lis2mdl = Adafruit_LIS2MDL(12345);

//set pins for sda and scl of the i2c connection
#define SDA 16
#define SCL 17



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
  Wire.begin(SDA,SCL);
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
  magnet.setDescription("3-Achsen Magnetfeldsensor MLX90393");

  //View
  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Graph"); 
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Einfach");//Create a "view"
  PhyphoxBleExperiment::View thirdView;
  thirdView.setLabel("Multi");//Create a "view"


  //Graph
  PhyphoxBleExperiment::Graph xGraph;    
  xGraph.setLabel("x");
  xGraph.setUnitX("s");
  xGraph.setUnitY("mT");
  xGraph.setLabelX("Zeit");
  xGraph.setLabelY("B");
  xGraph.setChannel(1,2);
  xGraph.setColor("00ff04");


  PhyphoxBleExperiment::Graph yGraph;    
  yGraph.setLabel("y");
  yGraph.setUnitX("s");
  yGraph.setUnitY("mT");
  yGraph.setLabelX("Zeit");
  yGraph.setLabelY("B");
  yGraph.setChannel(1,3);
  yGraph.setColor("00a5ff");


  PhyphoxBleExperiment::Graph zGraph;    
  zGraph.setLabel("z");
  zGraph.setUnitX("s");
  zGraph.setUnitY("mT");
  zGraph.setLabelX("Zeit");
  zGraph.setLabelY("B");
  zGraph.setChannel(1,4);
  zGraph.setColor("ecfa00");


  PhyphoxBleExperiment::Graph sumGraph;    
  sumGraph.setLabel("Gesamt");
  sumGraph.setUnitX("s");
  sumGraph.setUnitY("mT");
  sumGraph.setLabelX("Zeit");
  sumGraph.setLabelY("B");
  sumGraph.setChannel(1,5);

  PhyphoxBleExperiment::Graph multiGraph;
  multiGraph.setLabel("y");
  multiGraph.setUnitX("s");
  multiGraph.setUnitY("mT");
  multiGraph.setLabelX("Zeit");
  multiGraph.setLabelY("B");    
  multiGraph.setChannel(1,5);
  multiGraph.setColor("ffffff");

  PhyphoxBleExperiment::Graph::Subgraph xData;
  xData.setChannel(1,2);
  xData.setColor("00ff04");

  PhyphoxBleExperiment::Graph::Subgraph yData;
  yData.setChannel(1,3);
  yData.setColor("00a5ff");

  PhyphoxBleExperiment::Graph::Subgraph zData;
  zData.setChannel(1,4);
  zData.setColor("ecfa00");

  multiGraph.addSubgraph(xData);
  multiGraph.addSubgraph(yData);
  multiGraph.addSubgraph(zData);

  
  PhyphoxBleExperiment::Value bx;         //Creates a value-box.
  bx.setLabel("B-x");                  //Sets the label
  bx.setPrecision(2);                     //The amount of digits shown after the decimal point.
  bx.setUnit("mT");                        //The physical unit associated with the displayed value.
  bx.setColor("00ff04");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  bx.setChannel(2);
  bx.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value by;         //Creates a value-box.
  by.setLabel("B-y");                  //Sets the label
  by.setPrecision(2);                     //The amount of digits shown after the decimal point.
  by.setUnit("mT");                        //The physical unit associated with the displayed value.
  by.setColor("00a5ff");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  by.setChannel(3);
  by.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value bz;         //Creates a value-box.
  bz.setLabel("B-z");                  //Sets the label
  bz.setPrecision(2);                     //The amount of digits shown after the decimal point.
  bz.setUnit("mT");                        //The physical unit associated with the displayed value.
  bz.setColor("ecfa00");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  bz.setChannel(4);
  bz.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value bg;         //Creates a value-box.
  bg.setLabel("Gesamt");                  //Sets the label
  bg.setPrecision(2);                     //The amount of digits shown after the decimal point.
  bg.setUnit("mT");                        //The physical unit associated with the displayed value.
  bg.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  bg.setChannel(5);
  bg.setXMLAttribute("size=\"2\"");

  firstView.addElement(xGraph);            //attach graph to view
  firstView.addElement(yGraph);            //attach graph to view
  firstView.addElement(zGraph);            //attach graph to view

  secondView.addElement(bx);
  secondView.addElement(by);
  secondView.addElement(bz);
  secondView.addElement(bg);

  thirdView.addElement(multiGraph);

  magnet.addView(firstView);               //Attach view to experiment
  magnet.addView(secondView); 
  magnet.addView(thirdView); 
  
  
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
    lis2mdl.getEvent(&event);
    delay(50);

    xo=event.magnetic.x*0.001;
    yo=event.magnetic.y*0.001;
    zo=event.magnetic.z*0.001;
  }
  
  //Serial.println(buttonState);
  PhyphoxBLE::write(t, x, y, z, g);   

  delay(20);
}
