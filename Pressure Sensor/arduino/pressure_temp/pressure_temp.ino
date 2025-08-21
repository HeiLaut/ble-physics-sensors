
#include <Adafruit_BMP085.h>
#include <phyphoxBle.h> 



#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5


Adafruit_BMP085 bmp;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void)
{
   PhyphoxBLE::start("DruckSensor");
   PhyphoxBleExperiment druck;

   druck.setTitle("Barometer");
   druck.setCategory("Sensor-Boxen");
   druck.setDescription("Druck/Temperatur");

   PhyphoxBleExperiment::View graph;
   graph.setLabel("Graph"); 
   PhyphoxBleExperiment::View simple;
   simple.setLabel("Einfach");//Create a "view"
   
   PhyphoxBleExperiment::Graph presTemp;    
   presTemp.setLabel("p(T)");
   presTemp.setUnitX("*C");
   presTemp.setUnitY("hPa");
   presTemp.setLabelX("Temperatur");
   presTemp.setLabelY("Druck");
   presTemp.setChannel(2,3);

   PhyphoxBleExperiment::Graph presT;    
   presT.setLabel("p(t)");
   presT.setUnitX("s");
   presT.setUnitY("hPa");
   presT.setLabelX("Zeit");
   presT.setLabelY("Druck");
   presT.setChannel(1,3);

   PhyphoxBleExperiment::Graph tempT;    
   tempT.setLabel("p(T)");
   tempT.setUnitX("s");
   tempT.setUnitY("*C");
   tempT.setLabelX("Zeit");
   tempT.setLabelY("Temperatur");
   tempT.setChannel(1,2);

//   PhyphoxBleExperiment::Graph tempT2;    
//   tempT2.setLabel("p(T)");
//   tempT2.setUnitX("s");
//   tempT2.setUnitY("*C");
//   tempT2.setLabelX("Zeit");
//   tempT2.setLabelY("Temperatur 2");
//   tempT2.setChannel(1,4);
   
   PhyphoxBleExperiment::Value temperature;        
   temperature.setLabel("Temperatur");            
   temperature.setPrecision(2);                  
   temperature.setUnit("*C");                      
   temperature.setColor("FFFFFF");    
   temperature.setChannel(2);
   temperature.setXMLAttribute("size=\"2\"");

//   PhyphoxBleExperiment::Value temperature2;        
//   temperature2.setLabel("Temperatur2");            
//   temperature2.setPrecision(2);                  
//   temperature2.setUnit("*C");                      
//   temperature2.setColor("FFFFFF");    
//   temperature2.setChannel(4);
//   temperature2.setXMLAttribute("size=\"2\"");
//   
   PhyphoxBleExperiment::Value pres;        
   pres.setLabel("Druck");            
   pres.setPrecision(2);                  
   pres.setUnit("hPa");                      
   pres.setColor("FFFFFF");    
   pres.setChannel(3);
   pres.setXMLAttribute("size=\"2\"");

   graph.addElement(presTemp);
   graph.addElement(presT);
   graph.addElement(tempT);
//   graph.addElement(tempT2);
   
   simple.addElement(temperature);
   //simple.addElement(temperature2);
   simple.addElement(pres);
   

   druck.addView(simple);
   druck.addView(graph);

   PhyphoxBLE::addExperiment(druck);
   
   Wire.begin(14, 12);
   Serial.begin(115200);
   if (!bmp.begin()) {
      Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    }

  sensors.begin(); 
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW);
}

void loop(void)
{
  float t = 0.001 * (float)millis();
  float p = bmp.readPressure()*0.01;
  float temp = bmp.readTemperature();
 // sensors.requestTemperatures();
  //float t2 = sensors.getTempCByIndex(0);
  Serial.print("t(s), ");Serial.print(t); Serial.print(",");
  Serial.print("T(*C), ");Serial.print(temp); Serial.print(",");
  Serial.print("p(hPa), "); Serial.println(p,2);//Serial.print(",");
  //Serial.print("T2(*C), ");Serial.println(t2);
  PhyphoxBLE::write(t,temp,p);
    
  delay(500);
}
