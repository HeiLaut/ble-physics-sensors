#include <phyphoxBle.h>

const int analogInput1 = 36;
const int analogInput2 = 39;
float val1 = 0;
float val2 = 0;

float t0 = 0;
float t1 = 0;
float dt = 0; //darkeningtme
float tdif = 0; //time between two signals
float tdif0 = 0;
bool trigger = false;
float sig = 0;

float t0_2 = 0;
float t1_2 = 0;
float dt_2 = 0; //darkeningtme
float tdif_2 = 0; //time between two signals
float tdif0_2 = 0;
bool trigger_2 = false;
float sig_2 = 0;

float dt_1_2 = 0;
bool trigger_1_2 = false;
bool trigger_2_1 = false;

void setup() {
    PhyphoxBLE::start("Lichtschranke");
    PhyphoxBleExperiment lightBarrier;
    
    lightBarrier.setTitle("Lichtschranke");
    lightBarrier.setCategory("Sensor-Boxen");


    PhyphoxBleExperiment::View graph;
    graph.setLabel("Graph");

    PhyphoxBleExperiment::View simple;
    simple.setLabel("Einfach");

  
    PhyphoxBleExperiment::Graph sigGraph;
    sigGraph.setLabel("Signal");
    sigGraph.setUnitY("");
    sigGraph.setUnitX("s");
    sigGraph.setLabelX("Zeit t");
    sigGraph.setLabelY("");
    sigGraph.setColor("FFCC5C");
    sigGraph.setChannel(1,2);

    PhyphoxBleExperiment::Graph sigGraph2;
    sigGraph2.setLabel("Signal2");
    sigGraph2.setUnitY("");
    sigGraph2.setUnitX("s");
    sigGraph2.setLabelX("Zeit t");
    sigGraph2.setLabelY("");
    sigGraph2.setColor("FFCC5C");
    sigGraph2.setChannel(1,5);
    
    PhyphoxBleExperiment::Graph timGraph;
    timGraph.setLabel("Zeiten");
    timGraph.setUnitY("s");
    timGraph.setUnitX("s");
    timGraph.setLabelX("Laufzeit");
    timGraph.setLabelY("Verdunklungszeit");
    timGraph.setColor("FFCC5C");
    timGraph.setChannel(3,4);
    timGraph.setStyle("dots");

    PhyphoxBleExperiment::Value deltaT;
    deltaT.setLabel("Verdunklungszeit L1 t =");
    deltaT.setPrecision(3);
    deltaT.setUnit("s");
    deltaT.setColor("FFCC5C"); 
    deltaT.setChannel(3);
    deltaT.setXMLAttribute("size=\"2\"");

    PhyphoxBleExperiment::Value deltaT0;
    deltaT0.setLabel("Signallaufzeit L1 L2 t = ");
    deltaT0.setPrecision(3);
    deltaT0.setUnit("s");
    deltaT0.setColor("FFCC5C"); 
    deltaT0.setChannel(4);
    deltaT0.setXMLAttribute("size=\"2\"");

    simple.addElement(deltaT);
    simple.addElement(deltaT0);
    graph.addElement(sigGraph);
    graph.addElement(sigGraph2);
    graph.addElement(timGraph);
    lightBarrier.addView(graph);
    lightBarrier.addView(simple);
    PhyphoxBLE::addExperiment(lightBarrier);

    
    pinMode(analogInput1, INPUT); //assigning the input port
    pinMode(analogInput2, INPUT); //assigning the input port

    Serial.begin(115200); //BaudRate

}

void loop() {
    float t = 0.001 * (float)millis();
    bool s1 = true;
    bool s2 = true;
    val1 = analogRead(analogInput1);//reads the analog input
    val2 = analogRead(analogInput2);//reads the analog input

    if(val1 < 2000){
      s1 = false;
    }
  
    if(val2 <2000){
      s2 = false;
    }

    if(val1 > 3100 and !trigger and s1){
        sig = 1;
        tdif = t - tdif0;
        t0 = t;
        tdif0 = t;
        trigger = true;
        trigger_1_2 = true;
         if(trigger_2_1){
          dt_1_2 = t - tdif0_2;
          trigger_2_1 = false;
          Serial.println(dt_1_2);
        }
        //Serial.println(tdif);
        
    }
    if(val1 <3100 and trigger and s1){
      sig = 0;
      trigger = false;
      t1 = t;
      dt = t1-t0;
    }

    if(val2 > 3200 and !trigger_2 and s2){
        sig_2 = 1;
        tdif_2 = t - tdif0_2;
        t0_2 = t;
        tdif0_2 = t;
        trigger_2 = true;
        trigger_2_1 = true;
        if(trigger_1_2){
          dt_1_2 = t - tdif0;
          trigger_1_2 = false;
          Serial.println(dt_1_2);
        }
        }
        //Serial.println(tdif);
        
    
    if(val2 <3200 and trigger_2 and s2){
      sig_2 = 0;
      trigger_2= false;
      t1_2 = t;
      dt_2 = t1_2-t0_2;
    }
    
    PhyphoxBLE::write(t,val1,dt,dt_1_2,val2);
    //delay(0);


}
