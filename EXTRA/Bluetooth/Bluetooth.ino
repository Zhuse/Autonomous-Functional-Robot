//IMPORTANT NOTES:
//1
  //Before, uploading the code to the Arduino, 
  //disconnect the HC-06 module
  //since it shares the tx/rx pins and will interfere with the upload
  //Connect it back once the code has been uploaded successfully.
//2
  //Password for bluetooth: 1234
//3
  //To receive, connect your serial terminal program to COM5
#include <SoftwareSerial.h>
SoftwareSerial BT(0,1);
String message; //string that stores the incoming message

void setup()
{
  BT.begin(9600); //set baud rate
}

void loop()
{
  while(BT.available())
  {//while there is data available on the serial monitor
    message+=char(BT.read());//store string from serial command
  }
  if(!BT.available())
  {
    if(message!="")
    {//if data is available
      BT.println(message); //show the data
      message=""; //clear the data
    }
  }
  delay(5000); //delay
}
