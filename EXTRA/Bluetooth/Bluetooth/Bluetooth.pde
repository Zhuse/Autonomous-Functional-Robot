//IMPORTANT NOTES:
//1
  //Password for bluetooth: 1234
//2
  //To receive, connect your serial terminal program to COM5
//3
  //We have decided to account for the fact that multiple leap motion instructions will be contained within one serial output buffer line in our arduino code
import processing.serial.*;
Serial blueToothPort;
//Times below are in ms.
final int setupTime = 0; //bluetooth requires extra initial setup time. If you send serial output before waiting for this time,
//serial output will first be stored in a buffer then all sent at once (which is unsafe?)
final int safeSendMinimum = 0; //bluetooth first stores serial output in a buffer then sends output all at once. Wait till after this amount of time to be safe?

void setup(){
  //printArray(Serial.list());
  blueToothPort = new Serial(this, "COM5", 9600);  //Use COM5
  delay(setupTime);
}

//Sends A to bluetooth serial every 5 seconds
void draw(){
  blueToothPort.write(65);
  delay(safeSendMinimum);
}