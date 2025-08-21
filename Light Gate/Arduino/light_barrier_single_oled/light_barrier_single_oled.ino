#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128  // OLED width,  in pixels
#define SCREEN_HEIGHT 64  // OLED height, in pixels
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define BUTTON_PIN 27

int n = 0;
int spacing = 2;
float a = 0;
//variables to keep track of the timing of recent interrupts
int t1 = 0;
int t2 = 0;
int t3 = 0;
int t4 = 0;
int mode = 0;
int puffer = 0;
float dt=0;


bool s1 = false;
bool s2 = false;

void isr1() {
  n++;
  if(!s1 && !s2){
    t1=(int)millis();
    s1 = true;
  }else if(!s2){
    t2=(int)millis();
    s1=false;
    s2=true;
  }else if(!s1&&s2){
    t3=(int)millis();
    s1=true;
    s2=true;
  }else{
    t4=(int)millis();
    s1=0;
    s2=0;
  }
}


void setup() {
  Wire.begin(16,17);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1)
      ;
  }
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(22), isr1, CHANGE);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
if (!buttonState){ 
    Serial.println(mode);
    mode+=1;
    mode%=4;
    delay(100);
  }

  oled.clearDisplay();  // clear display
  
  oled.setTextSize(2);                                 // set text size
  oled.setTextColor(WHITE);                            // set text color
                              
if(mode == 3){
  oled.setTextSize(1.5);
  // float v1=0.02/((t2-t1)*0.001);
  // float v2=0.02/((t4-t3)*0.001);

  // float dt1 = (t2-t1)*0.01;
  // float dt2 = (t4-t3)*0.01;
  // float dt = (t3-t2)*0.01;
  // float a=(v2-v1)/(dt+dt1/2+dt2/2);
  oled.setTextSize(1);
  oled.setCursor(5,0);oled.print("Mehrfach");
  oled.setCursor(0,15);
  oled.print(t1*0.001,3);oled.print(" s");
  oled.setCursor(0, 25);
  oled.print(t2*0.001,3);oled.print(" s");
                           
  oled.setCursor(0,35);
  oled.print(t3*0.001,3);oled.print(" s");
  oled.setCursor(0,45);
  oled.print(t4*0.001,3);oled.print(" s");
 
}
if(mode == 1){
  oled.setTextSize(1);
  oled.setCursor(5,0);oled.print("Verdunklung");
  oled.setTextSize(2);
  float dt = 0;
  if(!s1&&s2){dt = (t2-t1)*0.001;}
  else{dt = (t4-t3)*0.001;}
  oled.setCursor(5,10);
  oled.print(dt,3);oled.print(" s");
  
  
}
if(mode ==0){
  oled.setTextSize(1);
  oled.setCursor(5,0);oled.print("Laufzeit");
  oled.setTextSize(2);
  float dt = abs((float)t3-(float)t1)*0.001;
  oled.setCursor(5,10);
  oled.print(dt,3);oled.print(" s");
}

if(mode ==2){
  if(puffer!=t1){
    dt = float(t1)-float(puffer);
  }
  oled.setTextSize(1);
  oled.setCursor(5,0);oled.print("Pendel");
  oled.setTextSize(2);

  oled.setCursor(5,10);
  oled.print(dt*0.001,3);oled.print(" s");
  oled.setCursor(5,40);
  oled.print(1/(dt*0.001),3);oled.print(" Hz");

  puffer = t1;

}

  oled.setTextSize(1);
  oled.setCursor(120,0);
  oled.print(mode);
  oled.display();
    
    
 // float values[5] = {t, signal1.high, s , signal1.ddt*0.001, signal1.dt*0.001};
  //PhyphoxBLE::write(&values[0], 5);  

  // Serial.print("t,");Serial.print(t,3);
  // Serial.print(",t1,");Serial.print(signal1.t0*0.001,3);
  // Serial.print(",dt,"); Serial.print(signal1.dt*0.001,3);
  // Serial.print(",s,");Serial.print(s,3);
  // Serial.print(",ddt,");Serial.println(signal1.ddt*0.001,3);
  delay(50);                                  


}
