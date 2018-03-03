import processing.serial.*;
Serial blueToothPort;

void setup(){
  //printArray(Serial.list());
  blueToothPort = new Serial(this, "COM5", 9600);  //Use COM5
}

//Sends A to bluetooth serial every 5 seconds
void draw(){
  blueToothPort.write(65);
  delay(5000);
}