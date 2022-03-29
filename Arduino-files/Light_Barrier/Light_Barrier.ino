#include <phyphoxBle.h>

const int analogInput1 = 36;
float val1 = 0;
float t0 = 0;
float t1 = 0;
float dt = 0; //darkeningtme

float tdif = 0; //time between two signals
float tdif0 = 0;
bool trigger = false;

void setup() {
    PhyphoxBLE::start("Lichtschranke");
    PhyphoxBleExperiment lightBarrier;
    
    lightBarrier.setTitle("Lichtschranke");
    lightBarrier.setCategory("Sensor-Boxen");


    PhyphoxBleExperiment::View graph;
    graph.setLabel("Graph");

    PhyphoxBleExperiment::Graph sigGraph;
    sigGraph.setLabel("Spannung 1");
    sigGraph.setUnitX("s");
    sigGraph.setLabelX("Zeit t");
    sigGraph.setColor("FFCC5C");
    sigGraph.setChannel(1,2);

    graph.addElement(sigGraph);
    lightBarrier.addView(graph);
    PhyphoxBLE::addExperiment(lightBarrier);

    
    pinMode(analogInput1, INPUT); //assigning the input port
    Serial.begin(115200); //BaudRate

}

void loop() {
    float t = 0.001 * (float)millis();

    val1 = analogRead(analogInput1);//reads the analog input
    if(val1 > 2000 and !trigger){
        tdif = t - tdif0;
        t0 = t;
        tdif0 = t;
        trigger = true;
        Serial.println(tdif);
        
    }
    if(val1 <2000 and trigger){
      trigger = false;
      t1 = t;
      dt = t1-t0;
      //Serial.println(dt);

    }
    
    PhyphoxBLE::write(t,val1);
    delay(5);


}
