#define INTERRUPT_ATTR IRAM_ATTR 


float rev = 0;
unsigned int rpm;
unsigned long oldtime = 0;
unsigned long times;
int interruptPin = 22;
//int signalPin1  = 22;
//int signalPin2  = 23;

void IRAM_ATTR ISR()  //interrupt service routine
{
  rev++;
}

void setup() {

  Serial.begin(115200);  // Baud Rate

  attachInterrupt(interruptPin,ISR,RISING);  //attaching the interrupt
}

void loop() {
  delay(1000);
  detachInterrupt(interruptPin);  //detaches the interrupt
  times = micros() - oldtime;                            //finds the time
  rpm = (rev / times) * 60000000;                        //calculates rpm
  oldtime = micros();                                    //saves the current time
  rev = 0;
  //int signalState1 = digitalRead(signalPin1);
  //int signalState2 = digitalRead(signalPin2);
  //Serial.print(signalState1);
  //Serial.print(", ");
  //Serial.println(signalState2);
  Serial.print("RPM is ");
  Serial.println(rpm);

  attachInterrupt(interruptPin,ISR,FALLING);
}


