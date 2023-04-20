#include <OneWire.h>
#include <DallasTemperature.h>
#include <phyphoxBle.h> 


// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 33

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

int deviceCount = 0;
float tempC1;
float tempC2;

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW);
  
  sensors.begin();  // Start up the library
  Serial.begin(115200);
  PhyphoxBLE::start("Thermometer");
  PhyphoxBleExperiment temperature; 

  temperature.setTitle("Thermometer");
  temperature.setCategory("Sensor-Boxen");
  temperature.setDescription("Dual Thermometer Sensors");

  //View
  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Graph"); 
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Einfach");//Create a "view"

  //Graph
  PhyphoxBleExperiment::Graph firstGraph;    
  firstGraph.setLabel("Temperaturverlauf");
  firstGraph.setUnitX("s");
  firstGraph.setUnitY("*C");
  firstGraph.setLabelX("Zeit");
  firstGraph.setLabelY("Temperatur");
  firstGraph.setChannel(1,2);

  PhyphoxBleExperiment::Graph secondGraph;    
  secondGraph.setLabel("Temperaturverlauf 2");
  secondGraph.setUnitX("s");
  secondGraph.setUnitY("*C");
  secondGraph.setLabelX("Zeit");
  secondGraph.setLabelY("Temperatur 2");
  secondGraph.setChannel(1,3);

  PhyphoxBleExperiment::Value t1;         //Creates a value-box.
  t1.setLabel("T1");                  //Sets the label
  t1.setPrecision(1);                     //The amount of digits shown after the decimal point.
  t1.setUnit("*C");                        //The physical unit associated with the displayed value.
  t1.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  t1.setChannel(2);
  t1.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value t2;         //Creates a value-box.
  t2.setLabel("T2");                  //Sets the label
  t2.setPrecision(1);                     //The amount of digits shown after the decimal point.
  t2.setUnit("*C");                        //The physical unit associated with the displayed value.
  t2.setColor("FFFFFF");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  t2.setChannel(3);
  t2.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::ExportSet mySet;       //Provides exporting the data to excel etc.
  mySet.setLabel("Temperatur");

  PhyphoxBleExperiment::ExportData exTime;
  exTime.setLabel("t(s)");
  exTime.setDatachannel(1);

  PhyphoxBleExperiment::ExportData exT1;
  exT1.setLabel("T1(*C)");
  exT1.setDatachannel(2);

  PhyphoxBleExperiment::ExportData exT2;
  exT2.setLabel("T2(*C)");
  exT2.setDatachannel(3);

  firstView.addElement(t1);
  firstView.addElement(firstGraph);            //attach graph to view

  firstView.addElement(t2);
  firstView.addElement(secondGraph);            //attach graph to view

  secondView.addElement(t1);
  secondView.addElement(t2);

  temperature.addView(firstView);               //Attach view to experiment
  temperature.addView(secondView); 
  
  mySet.addElement(exTime);
  mySet.addElement(exT1);
  mySet.addElement(exT2);
  temperature.addExportSet(mySet);  
  PhyphoxBLE::addExperiment(temperature);

}  
void loop(void)
{ 
  float t = 0.001 * (float)millis();
  // Send command to all the sensors for temperature conversion
  sensors.requestTemperatures(); 
  tempC1 = sensors.getTempCByIndex(0);
  tempC2 = sensors.getTempCByIndex(1);
  if(tempC1<-20){
    tempC1=0;
  }
  if(tempC2<-20){
    tempC2=0;
  }
  PhyphoxBLE::write(t, tempC1, tempC2);   


  Serial.print("t(s)");Serial.print(",");Serial.print(t);Serial.print(",");
  Serial.print("T1(*C)");Serial.print(",");Serial.print(tempC1,1);Serial.print(",");
  Serial.print("T2(*C)");Serial.print(",");Serial.println(tempC2,1);
  
  delay(200);
}
