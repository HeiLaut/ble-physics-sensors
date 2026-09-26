//#include <Wire.h>
#include <Adafruit_INA219.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>

#define SDA 15
#define SCL 13
#define NAME "Multimeter B"
Adafruit_INA219 ina219;
float interval = 20;

#define SERVICE_UUID   "8a740e1a-9aef-4962-a13f-53c20e827672"
#define CHAR_UUID_T    "832d0712-c19b-476b-a681-7a23881aaee6"  // time
#define CHAR_UUID_U    "78c243c1-0dce-4174-a147-e1d94109fc2a"  // signal time
#define CHAR_UUID_I    "fc72ec29-f239-44c5-a453-b7353cb54e62"  // shadowing time

BLECharacteristic *pCharT, *pCharU, *pCharI;
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

void receivedData();

void setup(void) {
  Serial.begin(115200);
  
  Wire.begin(SDA, SCL);

  BLEDevice::init(NAME);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID), 30);
  pCharT  = createChar(pService, CHAR_UUID_T);
  pCharU = createChar(pService, CHAR_UUID_U);
  pCharI = createChar(pService, CHAR_UUID_I);

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
  //uint32_t currentFrequency;
  
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
}
 
void loop(void) {

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
  
    String sT   = String(t,2);
    String sU   = String(loadvoltage,2);
    String sI   = String(current_mA,2);
   

    pCharT->setValue(sT.c_str()); pCharT->notify();
    pCharU->setValue(sU.c_str()); pCharI->notify();
    pCharI->setValue(sI.c_str()); pCharU->notify();
  
  
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
