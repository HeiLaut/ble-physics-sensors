#include <phyphoxBle.h>

const int analogInput1 = 36;
float val1 = 0;
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
    Serial.println(val1);
    PhyphoxBLE::write(t,val1);
    delay(10);


}
