#include <phyphoxBle.h>
#include "Adafruit_MLX90393.h"

Adafruit_MLX90393 sensor = Adafruit_MLX90393();
#define MLX90393_CS 10

//define pin for reset button
#define BUTTON_PIN 27 
//define the pins for the i2c connection
#define SDA 26
#define SCL 25
//when using the adafruit sensor you had to add the i2c adress, the dafault is 0x18
#define I2C_Adress 0x18

float xo = 0;
float yo = 0;
float zo = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW);
  Wire.begin(SDA,SCL);//17,16
  Serial.begin(115200);
  
  // assign inpmT pullup resistor to bmTton pin
  pinMode(BUTTON_PIN, INPUT_PULLUP);


  if (! sensor.begin_I2C(I2C_Adress)) {          // hardware I2C mode, can pass in address & alt Wire
  //if (! sensor.begin_SPI(MLX90393_CS)) {  // hardware SPI mode
    Serial.println("No sensor found ... check your wiring?");
    while (1) { delay(10); }
  }
  Serial.println("Found a MLX90393 sensor");

  sensor.setGain(MLX90393_GAIN_1X);

  // Set resolmTion, per axis. Aim for sensitivity of ~0.3 for all axes.
  sensor.setResolution(MLX90393_X, MLX90393_RES_17);
  sensor.setResolution(MLX90393_Y, MLX90393_RES_17);
  sensor.setResolution(MLX90393_Z, MLX90393_RES_16);

  // Set oversampling
  sensor.setOversampling(MLX90393_OSR_3);

  // Set digital filtering
  sensor.setFilter(MLX90393_FILTER_5);

  
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
  int buttonstate = digitalRead(BUTTON_PIN);
  sensor.readData(&x, &y, &z);

  x = x*0.001-xo;
  y = y*0.001-yo;
  z = z*0.001-zo;

  float g =sqrt(pow(x,2)+pow(y,2)+pow(z,2));
  Serial.print("t(s): ,");  Serial.print(t);
  Serial.print(",Bx(mT): ,");  Serial.print(x,3);
  Serial.print(",By(mT): ,");  Serial.print(y,3);
  Serial.print(",Bz(mT): ,");  Serial.print(z,3);
  Serial.print(",Bg(mT): ,");  Serial.println(g,2);

  if(buttonstate==0){
    sensor.readData(&x, &y, &z);
    delay(10);
    xo=x*0.001;
    yo=y*0.001;
    zo=z*0.001;
    delay(500);
  }
  
  //Serial.println(bmTtonState);
  PhyphoxBLE::write(t, x, y, z, g);   

  delay(20);
}
