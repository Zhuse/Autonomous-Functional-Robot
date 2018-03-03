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

String message; //string that stores the incoming message

void setup()
{
  Serial.begin(9600); //set baud rate
}

void loop()
{
  while(Serial.available())
  {//while there is data available on the serial monitor
    message+=char(Serial.read());//store string from serial command
  }
  if(!Serial.available())
  {
    if(message!="")
    {//if data is available
      Serial.println(message); //show the data
      message=""; //clear the data
    }
  }
  delay(5000); //delay
}
