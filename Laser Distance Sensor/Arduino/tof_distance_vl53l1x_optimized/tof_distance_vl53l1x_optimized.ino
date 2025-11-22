#include <phyphoxBle.h>
#include "Adafruit_VL53L1X.h"

#define BUTTON_PIN 27
#define READBAT_PIN 25
#define SDA 16
#define SCL 17

Adafruit_VL53L1X vl53 = Adafruit_VL53L1X();

float d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0;
float t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;

float offset = 0, t_offset = 0;
float distance = 0, velocity = 0;

float lastValidDist = 0;
const float MAX_JUMP = 30;

int stopped = 1;
int pressed = 0;
int synced = 0;

void setup() {
  Wire.begin(SDA, SCL);
  Serial.begin(115200);

  if (!vl53.begin(0x29, &Wire)) {
    Serial.println("Error init VL53L1X");
    while (1) {
      digitalWrite(LED_BUILTIN, LOW); delay(200);
      digitalWrite(LED_BUILTIN, HIGH); delay(200);
    }
  }

  vl53.startRanging();
  vl53.setTimingBudget(50);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  PhyphoxBLE::start("Laser-Distanz 0-400cm");
  PhyphoxBLE::experimentEventHandler = &newExperimentEvent;
  PhyphoxBLE::printXML(&Serial);

  PhyphoxBleExperiment entfernung;
  entfernung.setTitle("Laser-Entfernungsmesser");
  entfernung.setCategory("Sensor-Boxen");

  PhyphoxBleExperiment::View graphView, simpleView, velView;
  graphView.setLabel("Graph");
  simpleView.setLabel("Einfach");
  velView.setLabel("Geschwindigkeit");

  PhyphoxBleExperiment::Graph gDist;
  gDist.setLabel("s(t)");
  gDist.setUnitX("s");
  gDist.setUnitY("cm");
  gDist.setLabelX("Zeit");
  gDist.setLabelY("Weg");
  gDist.setChannel(1, 2);

  PhyphoxBleExperiment::Graph gVel;
  gVel.setLabel("v(t)");
  gVel.setUnitX("s");
  gVel.setUnitY("cm/s");
  gVel.setLabelX("Zeit");
  gVel.setLabelY("Geschwindigkeit");
  gVel.setChannel(1, 3);

  PhyphoxBleExperiment::Value dist, vel, bat;
  dist.setLabel("s");
  dist.setPrecision(2);
  dist.setUnit("cm");
  dist.setChannel(2);

  vel.setLabel("v");
  vel.setUnit("cm/s");
  vel.setPrecision(0);
  vel.setChannel(3);

  //------------- BATTERIESPANNUNG IN VOLT ----------------
  bat.setLabel("Batterie");
  bat.setPrecision(3);        // 3 Nachkommastellen für Spannung
  bat.setUnit("V");
  bat.setChannel(4);

  graphView.addElement(dist);
  graphView.addElement(gDist);
  graphView.addElement(bat);

  simpleView.addElement(dist);

  velView.addElement(gDist);
  velView.addElement(vel);
  velView.addElement(gVel);

  entfernung.addView(graphView);
  entfernung.addView(simpleView);
  entfernung.addView(velView);

  // ---------------- EXPORT -----------------
  PhyphoxBleExperiment::ExportSet exportSet;
  exportSet.setLabel("Distanz");

  PhyphoxBleExperiment::ExportData exTime;
  exTime.setLabel("t_s");
  exTime.setDatachannel(1);

  PhyphoxBleExperiment::ExportData exDist;
  exDist.setLabel("s_cm");
  exDist.setDatachannel(2);

  PhyphoxBleExperiment::ExportData exVel;
  exVel.setLabel("v_cm_per_s");
  exVel.setDatachannel(3);

  exportSet.addElement(exTime);
  exportSet.addElement(exDist);
  exportSet.addElement(exVel);

  entfernung.addExportSet(exportSet);

  PhyphoxBLE::addExperiment(entfernung);
}

void loop() {

  static uint32_t lastPress = 0;
  int buttonState = digitalRead(BUTTON_PIN);

  if (!buttonState && (millis() - lastPress > 200) && stopped) {
    lastPress = millis();
    pressed = 1;
    offset = vl53.distance() / 10.0f;
  }

  // ---------------- BATTERIE IN VOLT BERECHNEN --------------
  float adc = analogRead(READBAT_PIN);
  float v_adc = adc * (3.3f / 4095.0f);  // Spannung am ADC-Pin
  float batValue = v_adc * 2.0f;         // Faktor für Spannungsteiler
  //-----------------------------------------------------------

  if (vl53.dataReady() && (!stopped || !synced)) {

    float raw = vl53.distance();
    if (raw == 0 || raw > 4000) return;

    distance = raw / 10.0f - offset;

    if (lastValidDist != 0 && fabs(distance - lastValidDist) > MAX_JUMP) {
      return;
    }
    lastValidDist = distance;

    float t = millis() * 0.001f - t_offset;

    d1 = d2; d2 = d3; d3 = d4; d4 = d5; d5 = distance;
    t1 = t2; t2 = t3; t3 = t4; t4 = t5; t5 = t;

    if ((t5 - t4) <= 0 || (t4 - t3) <= 0 || (t3 - t2) <= 0 || (t2 - t1) <= 0) {
      velocity = 0;
    } else {
      velocity =
        ((d5 - d4) / (t5 - t4) +
         (d4 - d3) / (t4 - t3) +
         (d3 - d2) / (t3 - t2) +
         (d2 - d1) / (t2 - t1)) * 0.25f;
    }

    Serial.print("t,"); Serial.print(t);
    Serial.print("  s(cm),"); Serial.print(distance);
    Serial.print("  v(cm/s),"); Serial.print(velocity);
    Serial.print("  bat(V),"); Serial.println(batValue, 3);

    PhyphoxBLE::write(t, distance, velocity, batValue);

    vl53.clearInterrupt();
  }
}

void newExperimentEvent() {

  if (PhyphoxBLE::eventType == 1) {
    if (pressed) {
      t_offset = millis() * 0.001f;
      pressed = 0;
    }
    stopped = 0;
  }

  if (PhyphoxBLE::eventType == 0 || PhyphoxBLE::eventType == 2) {
    stopped = 1;
  }

  if (PhyphoxBLE::eventType == 255) {
    synced = 1;
  }
}
