// This is a basic web serial template for p5.js. Please see:
// https://makeabilitylab.github.io/physcomp/communication/p5js-serial
//
// By Jon E. Froehlich
// @jonfroehlich
// http://makeabilitylab.io/
//


let pHtmlMsg;
let serialOptions = { baudRate: 115200  };
let serial;
let x = []; //data for x-Axis
let y = []; //data for y-Axis
let y2 = []; //second y-Axis data

let labelX, labelY, labelY2
let dataTable;
let receivedData = false;
let data = []; //list of incoming data
let pause = false;

function setup() {
  createCanvas(800,150);

  // Setup Web Serial using serial.js
  serial = new Serial();
  serial.on(SerialEvents.CONNECTION_OPENED, onSerialConnectionOpened);
  serial.on(SerialEvents.CONNECTION_CLOSED, onSerialConnectionClosed);
  serial.on(SerialEvents.DATA_RECEIVED, onSerialDataReceived);
  serial.on(SerialEvents.ERROR_OCCURRED, onSerialErrorOccurred);

  // If we have previously approved ports, attempt to connect with them
  serial.autoConnectAndOpenPreviouslyApprovedPort(serialOptions);

  // Add in a lil <p> element to provide messages. This is optional
  pHtmlMsg = createP("No Serial Device Connected");

  pauseButton = createButton('Run/ Pause (p)');
  pauseButton.position(800, 10);
  pauseButton.style("width","150px")
  pauseButton.mousePressed(togglePause);

  resetButton = createButton('Reset Data (r)');
  resetButton.position(800, 60);
  resetButton.style("width","150px")

  resetButton.mousePressed(resetData);

  serialButton = createButton('Open Serial Connection');
  serialButton.position(10,10);
  serialButton.mousePressed(openSerialPort)

  csvButton = createButton('Save CSV-File');
  csvButton.position(800,110);
  csvButton.style("width","150px")

  csvButton.mousePressed(saveCsv);
  dataSlider = createSlider(100, 5000, 1000, 100);
  dataSlider.position(10, 650);
  dataSlider.style('width', '300px');

  closeButton = createButton('Close Serial Connection');
  closeButton.position(0,720);
  closeButton.style('font-size','10px');
  closeButton.style('background-color','#8B0000');
  closeButton.mousePressed(closeSerialPort);

  dataTable = new p5.Table();

}

function draw() {

  if(serial.isOpen()){
    serialButton.position(-300,0);
  }else{
    serialButton.position(10,10);
  };

  background(220);
  let len = dataSlider.value();
  if(x.length>len){
  x = x.slice(x.length-len)
  y = y.slice(y.length-len)
}
  if(!pause){
   drawDia();
   pauseButton.style("background-color","#008CBA");

  }

  if(pause){
    pauseButton.style("background-color","green");
  }
  drawSerialData();
  textSize(16);
  textAlign(CENTER)
  text(dataSlider.value(),350,120);
  text("Number of Data Points",160,100);
}

function createButtons(){
  for(let i = 0; i < data.length-1; i  = i + 2){
    let bx = createButton('x');
    let by = createButton('y');
    bx.position(20+i*100, 580);
    by.position(50+i*100, 580);
    bx.style("font-size","14px");
    bx.style("padding","5px 5px");
    by.style("font-size","14px");
    by.style("padding","5px 5px");
    bx.mousePressed(function(){selectData('x',i)});
    by.mousePressed(function(){selectData('y',i)});
  }
}

function drawSerialData(){
  textSize(20);
  textAlign(LEFT);
  for(let i = 0; i < data.length-1; i = i + 2){
    text(data[i] + ':',20+100*i,30);
    text(data[i+1],70+110*i,30);
  }
}
/**
00px; * Callback function by serial.js when there is an error on web serial
 *
 * @param {} eventSender
 */
 function onSerialErrorOccurred(eventSender, error) {
  console.log("onSerialErrorOccurred", error);
  pHtmlMsg.html(error);
}

function onSerialConnectionOpened(eventSender) {
  console.log("onSerialConnectionOpened");
  pHtmlMsg.html("Serial connection opened successfully");
}


function onSerialConnectionClosed(eventSender) {
  console.log("onSerialConnectionClosed");
  receivedData = false;
}

function selectData(ax,dat){
  if(ax == 'x'){
    paramX = dat;
  }else{
    paramY = dat;
  }
  resetData()
}

function onSerialDataReceived(eventSender, newData) {
  data = newData.split(',');
  //called first time data are received and creates buttons for each data.

  if(!receivedData){
    createButtons();
    // parameters for the axis, default 0 - time, and 2 first data
    paramX = 0;
    paramY = 2;
    receivedData=true;

  }
  if(!pause){
  labelX=data[paramX];
  x.push(data[paramX+1]);
  labelY=data[paramY];
  y.push(data[paramY+1]);
  }
}

/**
 * Called automatically by the browser through p5.js when connect-button is clicked
 */
function openSerialPort() {
  if (!serial.isOpen()) {
    serial.connectAndOpen(null, serialOptions);
    createButtons();
  }

}

function closeSerialPort(){
  serial.close();
  resetData();
}

function keyPressed(){
  if(key=='p'){
   togglePause();
  }
  if(key == 'r'){
    resetData();
  }
}



function togglePause(){
   if(pause){
      pause = false;
    }else{
    pause = true;}
    print(pause)
}

function resetData(){
    x=[];
    y=[];
    drawDia();
    print('reset');

}

function addData(xD,yD){
  let newRow = dataTable.addRow();
  newRow.setString(labelX, xD);
  newRow.setString(labelY, yD);
}
function saveCsv(){
  dataTable.clearRows()
  dataTable.removeColumn(labelX)
  dataTable.removeColumn(labelY)

  dataTable.addColumn(labelX);
  dataTable.addColumn(labelY);
  for(let i=0;i<x.length;i++){
    addData(x[i],y[i]);
  }
  saveTable(dataTable, 'data.csv');

}
