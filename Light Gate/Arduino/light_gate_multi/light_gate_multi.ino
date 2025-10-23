//Version 0.8

#include <phyphoxBle.h>

// Define pin numbers for the incoming signals and the detection pin
#define SIGNAL_A_PIN 17//22// 17 //pin for incoming signal A 
#define SIGNAL_B_PIN 16// 21// 16 //pin for incoming signal B
#define DETECT_PIN 26 //pin for detect second light gate
#define LED_PIN 13 //pin for indication LED

#define BLENAME "Lichtschranke #1"
// Structure to represent a signal with its associated pin and timing information
struct Signal{
    const int pin; // Pin number associated with the signal
    //bool high1;    // State to track the first HIGH signal
    int t1;        // Timestamp for the first event (rising or falling edge)
    int t2;        // Timestamp for the second event (rising or falling edge)

};
// Flag to recognize if it's a single light gate or a magnet on RJ45 input
bool single = 0;

float t_offset = 0;
// Initialize signalA and signalB with their respective pins and initial states
Signal signalA = {SIGNAL_A_PIN,0,0};
Signal signalB = {SIGNAL_B_PIN,0,0};

// Time buffers to calculate different time intervals
int puffer1 = 0; //Time puffer to get runtime_A_A 
int puffer2 = 0; //Time puffer to get pendulum time 
float T = 0;  // Pendulum time
int n = 0;  // Counter for the number of times the light gate A is interrupted
int n_puffer = 0; //puffer to check, if a change happend.
float t = 0; //time passed
float runtime_A_A=0; // Runtime between two rising signals of light gate A

int spokes = 20; //number of spokes of encoder wheel
int diameter = 53; //diameter of encoder wheel

// Interrupt service routine for light gate A
void IRAM_ATTR ISR1() {
   // Increment the counter for each interrupt
    n++;
  //evantually turn into !digitalRead, depends on the sensor module
   if(digitalRead(SIGNAL_A_PIN)){
     signalA.t1 = (int)millis(); // Record the timestamp of the first event
  }else{
    signalA.t2 = (int)millis(); // Record the timestamp of the second event
  }
}

// Interrupt service routine for light gate B
void IRAM_ATTR ISR2() {
  //evantually turn into !digitalRead

  if(digitalRead(SIGNAL_B_PIN)){
    signalB.t1 = (int)millis();  // Record the timestamp of the first event
  }else{
    signalB.t2 = (int)millis(); // Record the timestamp of the second event
  }
  
}

// Setup function to initialize the system
void setup() {
  Serial.begin(115200);
  // Set the signal pins to input mode with internal pull-up resistors  
  pinMode(signalA.pin, INPUT_PULLUP);
  pinMode(signalB.pin, INPUT_PULLUP);
  pinMode(DETECT_PIN,INPUT_PULLUP);
  pinMode(LED_PIN,OUTPUT);

  delay(2000);

  Serial.println("-----------------------------");
  Serial.println("Lightgate Version 0.8");
  Serial.println("-----------------------------");

  delay(1000);

  // Check if the second light gate or magnet is connected via RJ45
  single = digitalRead(DETECT_PIN);
  
  // Set up interrupt routines for the light gates
    attachInterrupt(SIGNAL_A_PIN, ISR1, CHANGE); // Interrupt for light gate A on any change (rising or falling)

  if(single){
    //Set up phyphox for single light gate use
    single_phyphox();
  }else{
    //Set up phyphox for dual light gate use
    dual_phyphox();
    //Set up interrupt routine for light gate B
    attachInterrupt(SIGNAL_B_PIN, ISR2, CHANGE);  // Interrupt for light gate B on any change (rising or falling)
  }
}
// Main loop function that runs repeatedly
void loop() {
  if(single){
   single_loop(); // Run the single light gate loop if only one light gate is present
  }
  else{
    dual_loop();  // Run the dual light gate loop if both light gates are present
  }
    
  delay(10); // Delay to reduce the loop execution rate
}

// Function to handle the logic when operating with a single light gate
void single_loop(){
  // Calculate the current time in seconds since the program started
  t = 0.001 * (float)millis() - t_offset; 
  // Calculate the darkening time, which is the time the light gate is blocked
  float darkeningA = abs((float)signalA.t1 - (float)signalA.t2)*0.001;

  // Check if a new event has occurred since the last loop iteration
  if(puffer1 != signalA.t1){
    // Calculate the runtime between two consecutive rising edges of signal A
    runtime_A_A = (float(signalA.t1) - float(puffer1)) * 0.001;
    // If this is not the first event, calculate the period of the pendulum
    if(puffer2 != puffer1){
      T = (float(signalA.t1) - float(puffer2)) * 0.001;
    }
    // Update the time buffer for the next period calculation
    puffer2 = puffer1;
  }
  // Update the time buffer to the latest timestamp of signal A
  puffer1 = signalA.t1;
  // Calculate the distance based on the number of interruptions
  float dist = PI*diameter*n/(2*spokes)*0.1;
  
  // turn LED on if no signal is detected and off if a signal is detected
  if(!digitalRead(SIGNAL_A_PIN)){
    digitalWrite(LED_PIN, LOW);
  }else{
    digitalWrite(LED_PIN, HIGH);
  }
  if(n_puffer != n){
  // Send the calculated values to the phyphox app via BLE
  

  PhyphoxBLE::write(t, runtime_A_A, darkeningA, T, dist);  
  // Output the calculated values to the serial monitor for debugging
  Serial.print(t,3);Serial.print(",");
  Serial.print(darkeningA,3);Serial.print(",");
  Serial.print(runtime_A_A,3);Serial.print(",");
  Serial.print(T,3);Serial.print(",");
  Serial.println(dist);
  }
  n_puffer = n;
}

// Function to handle the logic when operating with dual light gates
void dual_loop(){
  // Calculate the darkening time for each light gate
  float darkeningA = abs((float)signalA.t1 - (float)signalA.t2)*0.001;
  float darkeningB = abs((float)signalB.t1 - (float)signalB.t2)*0.001; 
  // Calculate the runtime between the rising edges of both light gates
  float runtime_A_B = abs(signalA.t1 - signalB.t1) * 0.001;

  // Output the calculated values to the serial monitor for debugging
  Serial.print(darkeningA,3);Serial.print(",");
  Serial.print(darkeningB,3);Serial.print(",");
  Serial.println(runtime_A_B,3);

  // Send the calculated values to the phyphox app via BLE
  PhyphoxBLE::write(runtime_A_B, darkeningA, darkeningB);  
}

// Function to set up the phyphox BLE experiment for a single light gate
void single_phyphox(){
  // Initialize the BLE experiment with a title
  PhyphoxBLE::start(BLENAME);
  PhyphoxBLE::experimentEventHandler = &newExperimentEvent;
  PhyphoxBLE::printXML(&Serial);
  // Create a new experiment object
  PhyphoxBleExperiment lightGate;

  // Set the properties of the experiment
  lightGate.setTitle("Lichtschranke");
  lightGate.setCategory("Sensor-Boxen");
  lightGate.setDescription("Laufzeit, Verdunklungszeit, Pendel, Speichenrad");
  lightGate.numberOfChannels = 5;

  // Create views and values for the experiment
  PhyphoxBleExperiment::View mtimes; // View for measured times
  mtimes.setLabel("Laufzeit");

  PhyphoxBleExperiment::View pendulum; // View for pendulum measurements
  pendulum.setLabel("Pendel");

  PhyphoxBleExperiment::View wheel; // View for wheel measurements
  wheel.setLabel("Speichenrad");

  // Define the values to be displayed in the phyphox app
  PhyphoxBleExperiment::Value runtime; // Runtime value
  runtime.setLabel("Signallaufzeit  t =");
  runtime.setPrecision(3);
  runtime.setUnit("s");
  runtime.setColor("FFCC5C");
  runtime.setChannel(2);
  runtime.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value darktime; // Darkening time value
  darktime.setLabel("Verdunklungszeit  t =");
  darktime.setPrecision(3);
  darktime.setUnit("s");
  darktime.setColor("FFCC5C");
  darktime.setChannel(3);
  darktime.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value period; // Pendulum period value
  period.setLabel("Schwingungsd.  T =");
  period.setPrecision(3);
  period.setUnit("s");
  period.setColor("FFCC5C");
  period.setChannel(4);
  period.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value distance; // Distance value
  distance.setLabel("Stecke  s =");
  distance.setPrecision(1);
  distance.setUnit("cm");
  distance.setColor("FFCC5C");
  distance.setChannel(5);
  distance.setXMLAttribute("size=\"2\"");
  
  PhyphoxBleExperiment::Graph periodGraph; // Graph for period over 
  periodGraph.setLabel("Schwingungsdauer");
  periodGraph.setUnitY("s");
  periodGraph.setUnitX("");
  periodGraph.setLabelX("n");
  periodGraph.setLabelY("T");
  periodGraph.setColor("76a5af");
  periodGraph.setChannel(5, 4);

  // Define a graph to be displayed in the phyphox app
  PhyphoxBleExperiment::Graph distGraph; // Graph for distance over time
  distGraph.setLabel("Weg");
  distGraph.setUnitY("cm");
  distGraph.setUnitX("s");
  distGraph.setLabelX("Zeit t");
  distGraph.setLabelY("s");
  distGraph.setColor("FFCC5C");
  distGraph.setChannel(1, 5);

  // PhyphoxBleExperiment::Edit distance0;
  // distance0.setLabel("StartStrecke");
  // distance0.setUnit("cm");
  // distance0.setChannel(1);
  // distance0.setDecimal(0);

  // Add the defined values to their respective views
  mtimes.addElement(runtime);
  mtimes.addElement(darktime);

  pendulum.addElement(period);
  pendulum.addElement(periodGraph);

  wheel.addElement(distance);
  wheel.addElement(distGraph);
  // wheel.addElement(distance0);
  // Add the views to the experiment
  lightGate.addView(mtimes);
  lightGate.addView(pendulum);
  lightGate.addView(wheel);

  // Add the experiment to the phyphox BLE interface
  PhyphoxBLE::addExperiment(lightGate);
}

// Function to set up the phyphox BLE experiment for dual light gates
void dual_phyphox(){
  // Initialize the BLE experiment with a title
  PhyphoxBLE::start(BLENAME);
  PhyphoxBLE::experimentEventHandler = &newExperimentEvent;
  PhyphoxBLE::printXML(&Serial);
  // Create a new experiment object
  PhyphoxBleExperiment lightGate;

  // Set the properties of the experiment
  lightGate.setTitle("Lichtschranke (2x)");
  lightGate.setCategory("Sensor-Boxen");
  lightGate.setDescription("Zwei Lichtschranken - Laufzeit, Verdunklungszeit");
  lightGate.numberOfChannels = 3;

  // Create a view for the times measured by the light gates
  PhyphoxBleExperiment::View times;
  times.setLabel("Laufzeit");

  // Define the values to be displayed in the phyphox app for dual light gates
  PhyphoxBleExperiment::Value runtime; // Runtime value between light gates A and B
  runtime.setLabel("Signallaufzeit A B  t =");
  runtime.setPrecision(3);
  runtime.setUnit("s");
  runtime.setColor("FFCC5C");
  runtime.setChannel(1);
  runtime.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value darktimeA; // Darkening time value for light gate A
  darktimeA.setLabel("Verdunklungszeit A  t =");
  darktimeA.setPrecision(3);
  darktimeA.setUnit("s");
  darktimeA.setColor("FFCC5C");
  darktimeA.setChannel(2);
  darktimeA.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value darktimeB; // Darkening time value for light gate B
  darktimeB.setLabel("Verdunklungszeit B  t =");
  darktimeB.setPrecision(3);
  darktimeB.setUnit("s");
  darktimeB.setColor("FFCC5C");
  darktimeB.setChannel(3);
  darktimeB.setXMLAttribute("size=\"2\"");

  // Add the defined values to the times view
  times.addElement(runtime);
  times.addElement(darktimeA);
  times.addElement(darktimeB);

  // Add the view to the experiment
  lightGate.addView(times);

  // Add the experiment to the phyphox BLE interface
  PhyphoxBLE::addExperiment(lightGate);
}

void newExperimentEvent(){
  if(PhyphoxBLE::eventType == 1 || PhyphoxBLE::eventType == 2){
    n = 0;
    t_offset = 0.001 * (float)millis();     
  }
}
// void receiveData(){
//    float received_n;
//    PhyphoxBLE::read(received_n);
//    n =(int)received_n;
// }
