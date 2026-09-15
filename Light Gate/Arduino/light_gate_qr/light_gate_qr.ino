#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---- Pin-Konfiguration ----
#define SIGNAL_PIN 26  
#define BUTTON_PIN 27
#define SDA_PIN 16   
#define SCL_PIN 17
#define LED_PIN 32

// ---- OLED ----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define NAME "Einzel-Lichtschranke A"

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

volatile int n = 0;
volatile int t1 = 0;
volatile int t2 = 0;
volatile int timeArray[3] = {0, 0, 0};
volatile float laufT = 0;
volatile float pendelT = 0;
volatile float pendelF = 0;
volatile bool newEvent = false;
volatile unsigned long lastRise = 0;
const unsigned long DEBOUNCE_US = 2000; // Startwert, ggf. anpassen

float verdT = 0;
bool stopped = 0;
bool cleared = 0;
bool synced = 0;

volatile float t = 0;
volatile float t_offset = 0;
// ---- Mode/Taster ----
// 0 = Laufzeit, 1 = Verdunklung, 2 = Pendel

int mode = 0;
bool lastButtonState = HIGH;   


#define SERVICE_UUID   "12345678-1234-1234-1234-123456789abc"
#define CHAR_UUID_T    "abcdef01-1234-1234-1234-123456789abc"  // time
#define CHAR_UUID_LT   "abcdef02-1234-1234-1234-123456789abc"  // signal time
#define CHAR_UUID_DT   "abcdef03-1234-1234-1234-123456789abc"  // shadowing time
#define CHAR_UUID_SD   "abcdef04-1234-1234-1234-123456789abc"  // period
#define CHAR_UUID_N    "abcdef05-1234-1234-1234-123456789abc"  // count

BLECharacteristic *pCharT, *pCharLT, *pCharDT, *pCharSD, *pCharN;
bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer)    { deviceConnected = true; }
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    BLEDevice::getAdvertising()->start();
  }
};

BLECharacteristic* createChar(BLEService* svc, const char* uuid) {
  BLECharacteristic* c = svc->createCharacteristic(uuid, BLECharacteristic::PROPERTY_NOTIFY);
  c->addDescriptor(new BLE2902());
  return c;
}

void IRAM_ATTR isr1() {
  n++;
  if (digitalRead(SIGNAL_PIN)) {
    unsigned long now = micros();
    if (now - lastRise < DEBOUNCE_US) return;  // Prellen ignorieren, timeArray NICHT verschieben
    lastRise = now;
    t1 = (int)now;
    timeArray[0] = timeArray[1];
    timeArray[1] = timeArray[2];
    timeArray[2] = t1;
    newEvent = true;
  } else {
    t2 = (int)micros();
  }
}


void setup() {
  Serial.begin(115200);

  BLEDevice::init("Lichtschranke 1");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID), 30);
  pCharT  = createChar(pService, CHAR_UUID_T);
  pCharLT = createChar(pService, CHAR_UUID_LT);
  pCharDT = createChar(pService, CHAR_UUID_DT);
  pCharSD = createChar(pService, CHAR_UUID_SD);
  pCharN = createChar(pService, CHAR_UUID_N);


  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();


  Wire.begin(SDA_PIN, SCL_PIN);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }

  pinMode(SIGNAL_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN), isr1, CHANGE);

  t_offset = 0;

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
   if (digitalRead(SIGNAL_PIN)) {
    digitalWrite(LED_PIN, LOW);  
  }else{
    digitalWrite(LED_PIN,HIGH);

  }

  bool reading = digitalRead(BUTTON_PIN);

  if (reading == LOW && lastButtonState == HIGH) {
    mode++;
    mode %= 4;
    delay(50);
    //Serial.println("press");
  }

  lastButtonState = reading;

   if(digitalRead(SIGNAL_PIN)){
    verdT = abs((float)t1 - (float)t2) * 0.000001f;
  }


  if (newEvent) {
    newEvent = false;
    t  = 0.000001f * (float)t1 - t_offset;
    laufT   = (timeArray[2] - timeArray[1]) * 0.000001f;
    pendelT = (timeArray[2] - timeArray[0]) * 0.000001f;
    pendelF = 1.0f / pendelT;
    Serial.println(String(t) + " | " + String(laufT) + " | " + String(verdT) + " | " +String(pendelT) + "|" + String(n/2));
     if (deviceConnected) {
    String sT       = String(t,4);
    String sLaufT   = String(laufT,4);
    String sVerdT   = String(verdT,4);
    String sPendelT = String(pendelT,4);//pendelT
    String sN = String(n/2);

    pCharN->setValue(sN.c_str()); pCharN->notify();
    pCharT->setValue(sT.c_str());       pCharT->notify();
    pCharLT->setValue(sLaufT.c_str());  pCharLT->notify();
    pCharDT->setValue(sVerdT.c_str());  pCharDT->notify();
    pCharSD->setValue(sPendelT.c_str()); pCharSD->notify();


    //delay(500);
  }
  }


 
 
  oled.clearDisplay();
  oled.setTextColor(WHITE);

  if(mode == 0){
    oled.setTextSize(1);
    oled.setCursor(5,0);  oled.print("Laufzeit");
    oled.setTextSize(2);
    oled.setCursor(5,10); oled.print(laufT,3); oled.print(" s");
  }
  if(mode == 1){
    oled.setTextSize(1);
    oled.setCursor(5,0);  oled.print("Verdunklung");
    oled.setTextSize(2);
    oled.setCursor(5,10); oled.print(verdT,4); oled.print(" s");
  }
  if(mode == 2){
    oled.setTextSize(1);
    oled.setCursor(5,0);  oled.print("Pendel T");
    oled.setTextSize(2);
    oled.setCursor(5,10); oled.print(pendelT,3); oled.print(" s");
  }
   if(mode == 3){
    oled.setTextSize(1);
    oled.setCursor(5,0);  oled.print("Pendel f");
    oled.setTextSize(2);
    oled.setCursor(5,10); oled.print(pendelF,3); oled.print(" Hz");
  }

  oled.setTextSize(1);
  oled.setCursor(120,0);
  oled.print(mode);
  oled.display();
}