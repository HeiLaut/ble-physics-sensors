#include <phyphoxBle.h>

const int analogInput1 = 36;
const int analogInput2 = 39;
float valA = 0;
float valB = 0;

bool trigA = false;
bool trigB = false;

float tAR = 0; //time a raising edge
float tAF = 0; //time a falling edge
float dtA = 0; //darkening time

float tBR = 0;
float tBF = 0;
float dtB = 0;

float dtABR = 0; //time between A and B raising points
float dtABF = 0; //time between A and B falling points


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
    sigGraph2.setChannel(1,3);
    
//    PhyphoxBleExperiment::Graph timGraph;
//    timGraph.setLabel("Zeiten");
//    timGraph.setUnitY("s");
//    timGraph.setUnitX("s");
//    timGraph.setLabelX("Laufzeit");
//    timGraph.setLabelY("Verdunklungszeit");
//    timGraph.setColor("FFCC5C");
//    timGraph.setChannel(3,4);
//    timGraph.setStyle("dots");

    PhyphoxBleExperiment::Value deltaTR;
    deltaTR.setLabel("Signallaufzeit L1 L2 R t =");
    deltaTR.setPrecision(3);
    deltaTR.setUnit("s");
    deltaTR.setColor("FFCC5C"); 
    deltaTR.setChannel(4);
    deltaTR.setXMLAttribute("size=\"2\"");

    PhyphoxBleExperiment::Value deltaTF;
    deltaTF.setLabel("Signallaufzeit L1 L2 F t = ");
    deltaTF.setPrecision(3);
    deltaTF.setUnit("s");
    deltaTF.setColor("FFCC5C"); 
    deltaTF.setChannel(5);
    deltaTF.setXMLAttribute("size=\"2\"");

//    PhyphoxBleExperiment::Value deltaA;
//    deltaA.setLabel("Verdunklungszeit A t =");
//    deltaA.setPrecision(3);
//    deltaA.setUnit("s");
//    deltaA.setColor("FFCC5C"); 
//    deltaA.setChannel(6);
//    deltaA.setXMLAttribute("size=\"2\"");
//
//    PhyphoxBleExperiment::Value deltaB;
//    deltaB.setLabel("Verdunklungszeit B t = ");
//    deltaB.setPrecision(3);
//    deltaB.setUnit("s");
//    deltaB.setColor("FFCC5C"); 
//    deltaB.setChannel(7);
//    deltaB.setXMLAttribute("size=\"2\"");


    simple.addElement(deltaTR);
    simple.addElement(deltaTF);
    //simple.addElement(deltaA);
   // simple.addElement(deltaB);
    graph.addElement(sigGraph);
    graph.addElement(sigGraph2);
    //graph.addElement(timGraph);
    lightBarrier.addView(graph);
    lightBarrier.addView(simple);
    PhyphoxBLE::addExperiment(lightBarrier);

    
    pinMode(analogInput1, INPUT); //assigning the input port
    pinMode(analogInput2, INPUT); //assigning the input port

    Serial.begin(115200); //BaudRate

}

void loop() {
    float t = 0.001 * (float)millis();

    valA = analogRead(analogInput1);//reads the analog input
    valB = analogRead(analogInput2);//reads the analog input

    if(valA > 1000 and !trigA){
      tAR = t;
      trigA = true;
           
    }

    if(valA < 1000 and trigA){
      tAF = t;
      dtA = t - tAR;
      trigA = false;
    }

    if(valB > 1000 and !trigB){
      tBR = t;
      trigB = true;
    }

    if(valB < 1000 and trigB){
      tBF = t;
      dtB = t - tBR;
      trigB = false;
    }
    
    dtABR = abs(tAR-tBR);
    dtABF = abs(tAF-tBF);
    



    PhyphoxBLE::write(t,valA,valB,dtABR,dtABF);//,dtA,dtB);
   // Serial.print(dtABR);
   // Serial.print(",");
   // Serial.println(dtABF);
    //delay(5);


}
