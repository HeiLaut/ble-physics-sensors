//#include <Wire.h>
#include <Adafruit_INA219.h>c:\Users\Heinrich\Nextcloud\Physik\DIY\Physics-Arduino-Sensors\Laser Distance Sensor\Arduino\tof_distance_vl53l1x\tof_distance_vl53l1x.ino
#include <phyphoxBle.h> 
#define SDA 15
#define SCL 13

Adafruit_INA219 ina219;

float interval = 20;

void receivedData();

void setup(void) {
  Wire.begin(SDA, SCL);
  Serial.begin(115200);
  
  uint32_t currentFrequency;
  
  //needed to select the scl and sda port for the lolin lite board Wire.begin(I2C_SDA, I2C_SCL)
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW);

  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1){
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    }
  }
 
  
  PhyphoxBLE::start("Multimeter");  
  PhyphoxBLE::configHandler = &receivedData;  
  
  PhyphoxBleExperiment multimeter;

  multimeter.setTitle("Multimeter");
  multimeter.setCategory("Sensor-Boxen");
  multimeter.setDescription("Spannungs und Strommessgerät");

  //View
   PhyphoxBleExperiment::View graph;
   graph.setLabel("Graph"); 
   PhyphoxBleExperiment::View simple;
   simple.setLabel("Einfach");//Create a "view"
   PhyphoxBleExperiment::View charac;
   charac.setLabel("Kennlinie");//Create a "view"
   PhyphoxBleExperiment::View powerView;
   powerView.setLabel("Leistung");//Create a "view"
  

   PhyphoxBleExperiment::Graph voltG;    
   voltG.setLabel("U(t)");
   voltG.setUnitX("s");
   voltG.setUnitY("V");
   voltG.setLabelX("Zeit");
   voltG.setColor("FF4000");
   voltG.setLabelY("Spannung");
   voltG.setChannel(1,2);

   PhyphoxBleExperiment::Graph currentG;    
   currentG.setLabel("I(t)");
   currentG.setUnitX("s");
   currentG.setUnitY("mA");
   currentG.setLabelX("Zeit");
   currentG.setLabelY("Stromstärke");
   currentG.setColor("00bfff");
   currentG.setChannel(1,3);
  
   PhyphoxBleExperiment::Graph powerGraph;    
   powerGraph.setLabel("P(U)");
   powerGraph.setUnitX("V");
   powerGraph.setUnitY("mW");
   powerGraph.setLabelX("Spannung");
   powerGraph.setLabelY("Leistung");
   powerGraph.setChannel(2,4);
   powerGraph.setStyle("dots");
   powerGraph.setColor("00FF00"); 

   PhyphoxBleExperiment::Graph powerGraphTime;    
   powerGraphTime.setLabel("P(t)");
   powerGraphTime.setUnitX("t");
   powerGraphTime.setUnitY("mW");
   powerGraphTime.setLabelX("Zeit");
   powerGraphTime.setLabelY("Leistung");
   powerGraphTime.setChannel(1,4);
   powerGraphTime.setColor("00FF00"); 
   powerGraphTime.setStyle("line");

   PhyphoxBleExperiment::Graph characteristic;    
   characteristic.setLabel("I(U)");
   characteristic.setUnitX("V");
   characteristic.setUnitY("mA");
   characteristic.setLabelX("Spannung");
   characteristic.setLabelY("Stromstärke");
   characteristic.setChannel(2,3);
   characteristic.setStyle("dots");

   PhyphoxBleExperiment::Value volt;        
   volt.setLabel("U");            
   volt.setPrecision(2);                  
   volt.setUnit("V");                      
   volt.setColor("FF4000");    
   volt.setChannel(2);
   volt.setXMLAttribute("size=\"2\"");

   PhyphoxBleExperiment::Value current;        
   current.setLabel("I");            
   current.setPrecision(2);                  
   current.setUnit("mA");                      
   current.setColor("00bfff");    
   current.setChannel(3);
   current.setXMLAttribute("size=\"2\"");

   PhyphoxBleExperiment::Value power;        
   power.setLabel("P");            
   power.setPrecision(2);                  
   power.setUnit("mW");                      
   power.setColor("00FF00");    
   power.setChannel(4);
   power.setXMLAttribute("size=\"2\"");

   PhyphoxBleExperiment::Edit sampleR;
   sampleR.setLabel("Abtastrate");
   sampleR.setUnit("ms");
   sampleR.setSigned(false);
   sampleR.setDecimal(false);
   sampleR.setChannel(1);
   sampleR.setXMLAttribute("min=\"10\"");

  //Export
   PhyphoxBleExperiment::ExportSet mySet;       //Provides exporting the data to excel etc.
   mySet.setLabel("Multimeter");

   PhyphoxBleExperiment::ExportData exTime;
   exTime.setLabel("t(s)");
   exTime.setDatachannel(1);

   PhyphoxBleExperiment::ExportData exVoltage;
   exVoltage.setLabel("U(V)");
   exVoltage.setDatachannel(2);
   
   PhyphoxBleExperiment::ExportData exCur;
   exCur.setLabel("I(mA)");
   exCur.setDatachannel(3);

   PhyphoxBleExperiment::ExportData exPower;
   exPower.setLabel("p(mW)");
   exPower.setDatachannel(4);

  
   graph.addElement(voltG);
   graph.addElement(currentG);
   graph.addElement(sampleR);
   
   simple.addElement(volt);
   simple.addElement(current);

   powerView.addElement(volt);
   powerView.addElement(current);
   powerView.addElement(power);
   powerView.addElement(powerGraphTime);
   powerView.addElement(powerGraph);


   charac.addElement(characteristic);

   multimeter.addView(simple);
   multimeter.addView(graph);
   multimeter.addView(charac);
   multimeter.addView(powerView);

   mySet.addElement(exTime);
   mySet.addElement(exVoltage);
   mySet.addElement(exCur);
   mySet.addElement(exPower);
   
   multimeter.addExportSet(mySet); 
   
   PhyphoxBLE::addExperiment(multimeter);
}

void loop(void) 
{
  float t = 0.001 * (float)millis();
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  power_mW = current_mA*loadvoltage;
  
  PhyphoxBLE::write(t, loadvoltage, current_mA, power_mW);   
  
  Serial.print("t(s)");Serial.print(",");
  Serial.print(t);Serial.print(",");
  Serial.print("U(V)");Serial.print(",");
  Serial.print(loadvoltage);Serial.print(",");
  Serial.print("I(mA)");Serial.print(",");
  Serial.print(current_mA);Serial.print(",");
  Serial.print("P(mW)");Serial.print(",");
  Serial.println(power_mW);
  
  delay(interval);
}


void receivedData() {           // get data from PhyPhox app
  float readInput;
  PhyphoxBLE::read(readInput);
  interval = readInput;
}
