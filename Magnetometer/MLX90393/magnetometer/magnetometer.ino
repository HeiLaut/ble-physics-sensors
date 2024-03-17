#include <phyphoxBle.h>
#include "Adafruit_MLX90393.h"

Adafruit_MLX90393 sensor = Adafruit_MLX90393();
#define MLX90393_CS 10

#define BUTTON_PIN 27 // GPIO27 pin connected to bmTton


float xo = 0;
float yo = 0;
float zo = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW);
  Wire.begin(17,16);
  Serial.begin(115200);
  
  // assign inpmT pullup resistor to bmTton pin
  pinMode(BUTTON_PIN, INPUT_PULLUP);


  if (! sensor.begin_I2C()) {          // hardware I2C mode, can pass in address & alt Wire
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
  bx.setPrecision(2);                     //The amount of digits shown after the decimal point.
  bx.setUnit("mT");                        //The physical unit associated with the displayed value.
  bx.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  bx.setChannel(2);
  bx.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value by;         //Creates a value-box.
  by.setLabel("B-y");                  //Sets the label
  by.setPrecision(2);                     //The amount of digits shown after the decimal point.
  by.setUnit("mT");                        //The physical unit associated with the displayed value.
  by.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  by.setChannel(3);
  by.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value bz;         //Creates a value-box.
  bz.setLabel("B-z");                  //Sets the label
  bz.setPrecision(2);                     //The amount of digits shown after the decimal point.
  bz.setUnit("mT");                        //The physical unit associated with the displayed value.
  bz.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  bz.setChannel(4);
  bz.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value bg;         //Creates a value-box.
  bg.setLabel("Gesamt");                  //Sets the label
  bg.setPrecision(2);                     //The amount of digits shown after the decimal point.
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
  int buttonstate = digitalRead(BUTTON_PIN);
  sensor.readData(&x, &y, &z);

  x = x*0.001-xo;
  y = y*0.001-yo;
  z = z*0.001-zo;

  float g =sqrt(pow(x,2)+pow(y,2)+pow(z,2));
  Serial.print("t(s): ,");  Serial.print(t);
  Serial.print(",Bx(mT): ,");  Serial.print(x,2);
  Serial.print(",By(mT): ,");  Serial.print(y,2);
  Serial.print(",Bz(mT): ,");  Serial.print(z,2);
  Serial.print(",Bg(mT): ,");  Serial.println(g,2);

  if(buttonstate==0){
    xo=x;
    yo=y;
    zo=z;
    delay(500);
  }
  
  //Serial.println(bmTtonState);
  PhyphoxBLE::write(t, x, y, z, g);   

  delay(20);
}
