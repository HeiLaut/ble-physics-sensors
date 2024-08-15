/*
===============================================================================================================
QMC5883LCompass.h Library XYZ Example Sketch
Learn more at [https://github.com/mprograms/QMC5883LCompass]

This example shows how to get the XYZ values from the sensor.

===============================================================================================================
Release under the GNU General Public License v3
[https://www.gnu.org/licenses/gpl-3.0.en.html]
===============================================================================================================
*/
#include <QMC5883LCompass.h>
#include <phyphoxBle.h>

QMC5883LCompass compass;



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW);
  Wire.begin(14,12);
  Serial.begin(115200);
  compass.init();
  //compass.setMode(0x01,0x04,0x10,0x40);
compass.setCalibrationOffsets(55.00, -56.00, -79.00);
compass.setCalibrationScales(0.97*0.5, 0.98*0.5, 1.06*0.5);

  
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
  thirdGraph.setLabel("y");
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
  bg.setUnit("uT");                        //The physical unit associated with the displayed value.
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

  // Read compass values
  compass.read();

  // Return XYZ readings
  x = compass.getX()*0.0001;
  y = compass.getY()*0.0001;
  z = compass.getZ()*0.0001;
  float g =sqrt(pow(x,2)+pow(y,2)+pow(z,2));
  Serial.print("t(s): ,");  Serial.print(t);
  Serial.print(",Bx(mT): ,");  Serial.print(x,2);
  Serial.print(",By(mT): ,");  Serial.print(y,2);
  Serial.print(",Bz(mT): ,");  Serial.print(z,2);
  Serial.print(",Bg(mT): ,");  Serial.println(g,2);

  PhyphoxBLE::write(t, x, y, z, g);   

  delay(50);
}
