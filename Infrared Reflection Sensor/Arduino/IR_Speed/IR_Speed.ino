/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-oled
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
int val = 0;              // variable to store the read value
#define SCREEN_WIDTH 128  // OLED width,  in pixels
#define SCREEN_HEIGHT 64  // OLED height, in pixels
int interruptPin = 5;
// create an OLED display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool change = 0;
int dt=0;
int t = 0;
void setup() {
  Serial.begin(115200);
  Wire.begin(14, 27);

  pinMode(interruptPin, INPUT);
  // initialize OLED display with I2C address 0x3C
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1)
      ;
  }
 delay(500);
  attachInterrupt(digitalPinToInterrupt(interruptPin), isr, CHANGE);
}

void loop() {
  int time=millis();

  if (change) {
    //static unsigned long last_interrupt_time = 0;
    //unsigned long interrupt_time = millis();
     // wait two seconds for initializing
    oled.clearDisplay();  // clear display

    oled.setTextSize(3);                                 // set text size
    oled.setTextColor(WHITE);                            // set text color
    
    oled.setCursor(0, 0);
    if(0.001*dt>=10){
      oled.print(0.001*dt,2);oled.println(" s");  // set text

    }else{
          oled.print(0.001*dt,3);oled.println(" s");  // set text
    }
                              
    oled.setCursor(0, 32);
    if(0.001*t>=10){
      oled.print(0.001*t,2);oled.println(" s");  // set text

    }else{
          oled.print(0.001*t,3);oled.println(" s");  // set text
    }
    //oled.print(0.001*t,3);oled.println(" s");
    oled.display();                                      // display on OLED
    change = false;
    Serial.print("t(s),");Serial.print(time*0.001,3);Serial.print(",T1(s),");Serial.print(t*0.001,3);Serial.print(",T2(s),");Serial.println(dt*0.001,3);
  }
}

void isr() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  static int count = 2;

  if(interrupt_time - last_interrupt_time > 2){
  //t = dt;
  if(count==2){
    dt = interrupt_time - last_interrupt_time;
    count=1;
  }
  else if(count==1){
     t = interrupt_time - last_interrupt_time;
     count=2;

  }
  last_interrupt_time = interrupt_time;
  change = true;
}

}