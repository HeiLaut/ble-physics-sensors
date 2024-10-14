
#include <Adafruit_AHTX0.h>
#include "ScioSense_ENS160.h"  // ENS160 library
#include <phyphoxBle.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define LEDPIN 22
#define SDA 17 //PIN for I2C SDA
#define SCL 16 //PIN for I2C SCL
#define ONE_WIRE_BUS 18


Adafruit_AHTX0 aht;
ScioSense_ENS160      ens160(ENS160_I2CADDR_1); //0x53..ENS160+AHT21
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float tempC;        //To store the temperature in C
float tempF;        //temp in F
float humidity;     //To store the humidity
float aqi;
float tvOC;
float eCO2;

void setup() {
  Serial.begin(115200);
  sensors.begin();

  Wire.begin(SDA, SCL);
  //Switch on LED for init
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

  Serial.print("ENS160...");
  ens160.begin();

  Serial.println(ens160.available() ? "done." : "failed!");
  if (ens160.available()) {
    Serial.print("\tStandard mode ");
    Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
  }

  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }

  //phyphox setup
  PhyphoxBLE::start("Umweltsensor");
  PhyphoxBleExperiment umwelt;

  umwelt.setTitle("Umweltsensor");
  umwelt.setCategory("Sensor-Boxen");
  umwelt.setDescription("Temperatur, Luftfeuchte, Luftqualität");

  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Umweltdaten");
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Graph");  //Create a "view"
  
  PhyphoxBleExperiment::Graph tempGraph;
  tempGraph.setLabel("Temperatur");
  tempGraph.setUnitX("s");
  tempGraph.setUnitY("°C");
  tempGraph.setLabelX("Zeit");
  tempGraph.setLabelY("Temperatur");
  tempGraph.setColor("ff0080");
  
  tempGraph.setChannel(0, 1);

  PhyphoxBleExperiment::Graph humGraph;
  humGraph.setLabel("Luftfeuchte");
  humGraph.setUnitX("s");
  humGraph.setUnitY("%");
  humGraph.setLabelX("Zeit");
  humGraph.setLabelY("Luftfeuchtigkeit");
  humGraph.setColor("0080FF");
  
  humGraph.setChannel(0, 2);
  
  PhyphoxBleExperiment::Graph aqiGraph;
  aqiGraph.setLabel("Luftqualität");
  aqiGraph.setUnitX("s");
  aqiGraph.setUnitY("");
  aqiGraph.setLabelX("Zeit");
  aqiGraph.setLabelY("AQI");
  aqiGraph.setColor("00ff00");  // Grün

  
  aqiGraph.setChannel(0, 3);

  PhyphoxBleExperiment::Graph ocGraph;
  ocGraph.setLabel("organische Verbindungen");
  ocGraph.setUnitX("s");
  ocGraph.setUnitY("ppb");
  ocGraph.setLabelX("Zeit");
  ocGraph.setLabelY("TVOC");
  ocGraph.setColor("ff00ff");  // Magenta

  ocGraph.setChannel(0, 4);

  PhyphoxBleExperiment::Graph co2Graph;
  co2Graph.setLabel("equivalent CO2");
  co2Graph.setUnitX("s");
  co2Graph.setUnitY("ppm");
  co2Graph.setLabelX("Zeit");
  co2Graph.setLabelY("eCO2");
  co2Graph.setColor("ffff00");  // Gelb

  co2Graph.setChannel(0, 5);

  PhyphoxBleExperiment::Value tempValue;
tempValue.setLabel("T");
tempValue.setPrecision(1);
tempValue.setUnit("°C");
tempValue.setColor("ff0080");  // Rot
tempValue.setChannel(1);
tempValue.setXMLAttribute("size=\"2\"");

PhyphoxBleExperiment::Value humValue;
humValue.setLabel("RH");
humValue.setPrecision(0);
humValue.setUnit("%");
humValue.setColor("0080FF");  // Blau
humValue.setChannel(2);
humValue.setXMLAttribute("size=\"2\"");

PhyphoxBleExperiment::Value aqiValue;
aqiValue.setLabel("AQI");
aqiValue.setPrecision(0);
aqiValue.setUnit("");
aqiValue.setColor("00ff00");  // Grün
aqiValue.setChannel(3);
aqiValue.setXMLAttribute("size=\"2\"");

PhyphoxBleExperiment::Value tvocValue;
tvocValue.setLabel("TVOC");
tvocValue.setPrecision(0);
tvocValue.setUnit("ppb");
tvocValue.setColor("ff00ff");  // Magenta
tvocValue.setChannel(4);
tvocValue.setXMLAttribute("size=\"2\"");

PhyphoxBleExperiment::Value eco2Value;
eco2Value.setLabel("eCO2");
eco2Value.setPrecision(0);
eco2Value.setUnit("ppm");
eco2Value.setColor("ffff00");  // Gelb
eco2Value.setChannel(5);
eco2Value.setXMLAttribute("size=\"2\"");

firstView.addElement(tempValue);
firstView.addElement(humValue);
firstView.addElement(aqiValue);
firstView.addElement(tvocValue);
firstView.addElement(eco2Value);

secondView.addElement(tempGraph);
secondView.addElement(humGraph);
secondView.addElement(aqiGraph);
secondView.addElement(ocGraph);
secondView.addElement(co2Graph);

umwelt.addView(firstView);
umwelt.addView(secondView);

PhyphoxBLE::addExperiment(umwelt); 

} // end void setup

/*--------------------------------------------------------------------------
  MAIN LOOP FUNCTION
  Cylce every 1000ms and perform measurement
 --------------------------------------------------------------------------*/

void loop() {
    ///// AHT20 start
  sensors.requestTemperatures(); 
  float rdTemp = sensors.getTempCByIndex(0);
  sensors_event_t humidity1, temp; //Tim had to change to  humidity1
  aht.getEvent(&humidity1, &temp);// populate temp and humidity objects with fresh data
  tempC = (temp.temperature);
  humidity = (humidity1.relative_humidity);
  if (ens160.available()) {
    // Give values to Air Quality Sensor.
    ens160.set_envdata(tempC, humidity);

    ens160.measure(true);
    ens160.measureRaw(true);
    aqi = ens160.getAQI();
    tvOC = ens160.getTVOC();
    eCO2 = ens160.geteCO2();

    Serial.print("AQI: ");Serial.print(aqi);Serial.print("\t");
    Serial.print("TVOC: ");Serial.print(tvOC);Serial.print("ppb\t");
    Serial.print("eCO2: ");Serial.print(eCO2);Serial.println("ppm\t");
  }

  Serial.print("Temperature: "); 
  Serial.print(rdTemp); 
  Serial.println(" degrees C");
  Serial.print("Temperature: "); 
  Serial.print("Humidity: "); 
  Serial.print(humidity); 
  Serial.println("% rH");

  PhyphoxBLE::write(rdTemp, humidity, aqi, tvOC, eCO2);

  delay(5000);

} //end void loop
