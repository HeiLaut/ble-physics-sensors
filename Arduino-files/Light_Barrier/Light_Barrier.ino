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
    deltaT.setLabel("t");
    deltaT.setPrecision(3);
    deltaT.setUnit("s");
    deltaT.setColor("FFCC5C"); 
    deltaT.setChannel(3);
    deltaT.setXMLAttribute("size=\"2\"");

    PhyphoxBleExperiment::Value deltaT0;
    deltaT0.setLabel("dt");
    deltaT0.setPrecision(3);
    deltaT0.setUnit("s");
    deltaT0.setColor("FFCC5C"); 
    deltaT0.setChannel(4);
    deltaT0.setXMLAttribute("size=\"2\"");

    simple.addElement(deltaT);
    simple.addElement(deltaT0);
    graph.addElement(sigGraph);
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

    val1 = analogRead(analogInput1);//reads the analog input
    val2 = analogRead(analogInput2);//reads the analog input

    if(val1 > 3100 and !trigger){
        sig = 1;
        tdif = t - tdif0;
        t0 = t;
        tdif0 = t;
        trigger = true;
        //Serial.println(tdif);
        
    }
    if(val1 <3100 and trigger){
      sig = 0;
      trigger = false;
      t1 = t;
      dt = t1-t0;

    }
    
    PhyphoxBLE::write(t,sig,tdif,dt);
    Serial.print(t,4);
    Serial.print(",");
    Serial.print(val1);
    Serial.print(",");
    Serial.println(val2);
    //delay(0);


}
