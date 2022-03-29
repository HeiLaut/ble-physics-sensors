#include <phyphoxBle.h>

const int analogInput1 = 36;
const int analogInput2 =39;
float Vout1 = 0.00;
float Vin1 = 0.00;
float Vout2 = 0.00;
float Vin2 = 0.00;
float R1 = 100000.00; // resistance of R1 (100K) 
float R2 = 10000.00; // resistance of R2 (10K) 
int val1 = 0;
int val2 = 0;

void setup(){
   PhyphoxBLE::start("Voltmeter");

   PhyphoxBleExperiment voltage;

   voltage.setTitle("DC-Voltmeter");
   voltage.setCategory("Sensor-Boxen");

   PhyphoxBleExperiment::View simple;
   simple.setLabel("Einfach");

   
   PhyphoxBleExperiment::View graph;
   graph.setLabel("Graph");

   
  PhyphoxBleExperiment::Graph v1Graph;      //Create graph which will p lot random numbers over time     
  v1Graph.setLabel("Spannung 1");
  v1Graph.setUnitX("s");
  v1Graph.setUnitY("V");
  v1Graph.setLabelX("Zeit t");
  v1Graph.setLabelY("Spannung U");
  v1Graph.setColor("FFCC5C");
  v1Graph.setChannel(1,2);


    
  PhyphoxBleExperiment::Graph v2Graph;      //Create graph which will p lot random numbers over time     
  v2Graph.setLabel("Spannung 2");
  v2Graph.setUnitX("s");
  v2Graph.setUnitY("V");
  v2Graph.setLabelX("Zeit t");
  v2Graph.setLabelY("Spannung U");
  v2Graph.setColor("88d8b0");
  v2Graph.setChannel(1,3);

  // Value of Voltage 1
   PhyphoxBleExperiment::Value v1;
   v1.setLabel("U_1");
   v1.setPrecision(1);
   v1.setUnit("V");
   v1.setColor("FFCC5C"); 
   v1.setChannel(2);
   v1.setXMLAttribute("size=\"2\"");

     // Value of Voltage 2
   PhyphoxBleExperiment::Value v2;
   v2.setLabel("U_2");
   v2.setPrecision(1);
   v2.setUnit("V");
   v2.setChannel(3);
   v2.setColor("88d8b0");                  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
   v2.setXMLAttribute("size=\"2\"");

   simple.addElement(v1);
   simple.addElement(v2);
   graph.addElement(v1Graph);
   graph.addElement(v2Graph);
   voltage.addView(simple);
   voltage.addView(graph);
   PhyphoxBLE::addExperiment(voltage);

  
   pinMode(analogInput1, INPUT); //assigning the input port
   pinMode(analogInput2, INPUT); //assigning the input port

   Serial.begin(115200); //BaudRate
}
void loop(){
    float t = 0.001 * (float)millis();

   
   val1 = analogRead(analogInput1);//reads the analog input
   val2 = analogRead(analogInput2);//reads the analog input

   Vout1 = (val1 *3.3/4095)+0.1356; //according to a problem with the analog input on the esp i have to add an correction of 0.1356 
   Vout2 = (val2 *3.3/4095)+0.1356;

   
  
   Vin1 = Vout1 / (R2/(R1+R2)); // formula for calculating voltage in i.e. GND
   Vin2 = Vout2 / (R2/(R1+R2)); // formula for calculating voltage in i.e. GND

   if(val1==0){
    Vin1=0;
   }
   if(val2==0){
    Vin2=0;
   }

//Serial.print("\t Voltage of the given source = ");

Serial.print(t);
Serial.print(",");
Serial.println(val2);
  PhyphoxBLE::write(t,Vin1,Vin2);

//Serial.print("ARead: "); Serial.println(val1);
delay(50); //for maintaining the speed of the output in serial moniter
}
