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

  csvButton.mousePressed(saveCsv)
  dataSlider = createSlider(100, 5000, 1000, 100);
  dataSlider.position(10, 630);
  dataSlider.style('width', '300px');

  dataTable = new p5.Table();

}

function draw() {

  if(serial.isOpen()){
    serialButton.position(-300,0);
  }

  background(220);
  let len = dataSlider.value();
  if(x.length>len){
  x = x.slice(x.length-len)
  y = y.slice(y.length-len)
}
  if(!pause){
   drawDia();
  }
  textSize(20);
  textAlign(CENTER);
  text(labelX + ':',30,30)
  text(x[x.length-1],90,30);
  text(labelY + ':',200,30)
  text(y[y.length-1],260,30);

  textSize(16);
  text(dataSlider.value(),350,110);
  text("Number of Data Points",160,80);
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
  let data = newData.split(',');
  //called first time data are received and creates buttons for each data.
  if(!receivedData){
    receivedData=true;
    dat0 = createButton('x-axis: '+ data[0]);
    dat0.position(800, 260);
    dat0.style("width","150px")
    dat0.mousePressed(function(){selectData('x',0)});

    dat1x = createButton('x-axis: '+ data[2]);
    dat1x.position(800, 160);
    dat1x.style("width","150px")
    dat1x.mousePressed(function(){selectData('x',2)});

    dat1y = createButton('y-axis: '+ data[2]);
    dat1y.position(955, 160);
    dat1y.style("width","150px")
    dat1y.mousePressed(function(){selectData('y',2)});

    dat2x = createButton('x-axis: '+data[4]);
    dat2x.position(800, 210);
    dat2x.style("width","150px")
    dat2x.mousePressed(function(){selectData('x',4)});

    dat2y = createButton('y-axis: '+ data[4]);
    dat2y.position(955, 210);
    dat2y.style("width","150px")
    dat2y.mousePressed(function(){selectData('y',4)});

    // parameters for the axis, default 0 - time, and 2 first data
    paramX = 0;
    paramY = 2;
  }
  //print(data)
  if(!pause){
  labelX=data[paramX];
  x.push(data[paramX+1]);
  labelY=data[paramY];
  y.push(data[paramY+1]);
  //print(data)
  }
}

/**
 * Called automatically by the browser through p5.js when connect-button is clicked
 */
function openSerialPort() {
  if (!serial.isOpen()) {
    serial.connectAndOpen(null, serialOptions);
  }

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
