#include <Wire.h>
#include <Adafruit_INA219.h>
#include <phyphoxBle.h> 

Adafruit_INA219 ina219;

float interval = 20;

void setup(void) {
  int sda = 15;
  int scl = 13;
  //needed to select the scl and sda port for the lolin lite board Wire.begin(I2C_SDA, I2C_SCL)
  Wire.begin(sda, scl);

  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW);
  
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

   PhyphoxBleExperiment::Graph voltG;    
   voltG.setLabel("U(t)");
   voltG.setUnitX("s");
   voltG.setUnitY("V");
   voltG.setLabelX("Zeit");
   voltG.setLabelY("Spannung");
   voltG.setChannel(1,2);

   PhyphoxBleExperiment::Graph currentG;    
   currentG.setLabel("I(t)");
   currentG.setUnitX("s");
   currentG.setUnitY("mA");
   currentG.setLabelX("Zeit");
   currentG.setLabelY("Stromstärke");
   currentG.setChannel(1,3);

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
   volt.setColor("FFFFFF");    
   volt.setChannel(2);
   volt.setXMLAttribute("size=\"2\"");

   PhyphoxBleExperiment::Value current;        
   current.setLabel("I");            
   current.setPrecision(2);                  
   current.setUnit("mA");                      
   current.setColor("FFFFFF");    
   current.setChannel(3);
   current.setXMLAttribute("size=\"2\"");

   PhyphoxBleExperiment::Value power;        
   power.setLabel("P");            
   power.setPrecision(2);                  
   power.setUnit("mW");                      
   power.setColor("FFFFFF");    
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
//   PhyphoxBleExperiment::ExportSet mySet;       //Provides exporting the data to excel etc.
//   mySet.setLabel("Multimeter");
//
//   PhyphoxBleExperiment::ExportData exTime;
//   exTime.setLabel("t(s)");
//   exTime.setDatachannel(1);
//
//   PhyphoxBleExperiment::ExportData exVoltage;
//   exVoltage.setLabel("U(V)");
//   exVoltage.setDatachannel(2);
   
//   PhyphoxBleExperiment::ExportData exCur;
//   exCur.setLabel("I(mA)");
//   exCur.setDatachannel(3);
//
 //  PhyphoxBleExperiment::ExportData exPower;
 //  exPower.setLabel("p(mW)");
 //  exPower.setDatachannel(4);


  
   graph.addElement(voltG);
   graph.addElement(currentG);
   graph.addElement(sampleR);
   
   simple.addElement(volt);
   simple.addElement(current);
   simple.addElement(power);

   charac.addElement(characteristic);

   multimeter.addView(simple);
   multimeter.addView(graph);
   multimeter.addView(charac);

//   mySet.addElement(exTime);
//   mySet.addElement(exVoltage);
//   mySet.addElement(exCur);
 //  mySet.addElement(exPower);
   
//   multimeter.addExportSet(mySet); 
   
   PhyphoxBLE::addExperiment(multimeter);


  Serial.begin(115200);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }
  

  uint32_t currentFrequency;
    
  Serial.println("Hello!");
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

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
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
//  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
//  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
//  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
//  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
//  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
//  Serial.println("");

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
