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
   pinMode(analogInput1, INPUT); //assigning the input port
   pinMode(analogInput2, INPUT); //assigning the input port

   Serial.begin(115200); //BaudRate
}
void loop(){
   
   val1 = analogRead(analogInput1);//reads the analog input
   val2 = analogRead(analogInput2);//reads the analog input

   Vout1 = (val1 * 3.3) / 4095; // formula for calculating voltage out i.e. V+, here 5.00
   Vout2 = (val2 * 3.3) / 4095; // formula for calculating voltage out i.e. V+, here 5.00

   Vin1 = Vout1 / (R2/(R1+R2)); // formula for calculating voltage in i.e. GND
   Vin2 = Vout2 / (R2/(R1+R2)); // formula for calculating voltage in i.e. GND
   //if (Vin1<0.09)//condition 
   {
   //  Vin1=0.00;//statement to quash undesired reading !
  }
 // if (Vin2<0.09)//condition 
   {
  //   Vin2=0.00;//statement to quash undesired reading !
  } 
//Serial.print("\t Voltage of the given source = ");
Serial.print("V1: ");
Serial.println(Vin1);
Serial.print("V2: ");
Serial.println(Vin2);
delay(100); //for maintaining the speed of the output in serial moniter
}
