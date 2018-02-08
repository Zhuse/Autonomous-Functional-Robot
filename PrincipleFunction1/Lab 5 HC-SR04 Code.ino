#include <Servo.h>

int angle = 0;
const int temperaturePort = 0; // 5;
const int echoPort = 9; // 6;
const int triggerPort = 10; // 7;
const int servoPort = 13; // 8;
const int sweepSpeed = 60; //should not be less than 50ms (pointless to update the angle otherwise since the minimum trigger time is 50µs)
const int nonStutteringIncrement = 1;  // how much to move the arduino every time we update its position 

Servo servo;
char turnCommand; //for externally controlling the servo

void setup() {
  Serial.begin (9600);
  pinMode(temperaturePort, INPUT);
  pinMode(echoPort, INPUT);
  pinMode(triggerPort, OUTPUT);
  servo.attach(servoPort);

  servo.write(angle); // initialize servo position
  delay(10);
}

void loop()
{

  if (Serial.available()) /// If data is available to read,
  {
    turnCommand = Serial.read();// read it and store it in turnCommand
    //if recieved input is 1, turn left
    if (turnCommand == '1' && angle + nonStutteringIncrement <= 180) //if value input is equals to d
    {
      angle += nonStutteringIncrement; 
      servo.write(angle); 
     }
     // else if recieved input is 2, turn right 
    if (turnCommand == '2' && angle - nonStutteringIncrement >= 0)
    {
      angle -= nonStutteringIncrement;
      servo.write(angle);
    }
  }

  triggerSensor();  // send the trigger signal to the ultrasonic sensor for the sensor to begin sending data 
  double temperature = getTemperature(); // get the temperature from the LM35
  double speedOfSound = getSpeedOfSound(temperature);  // calculate the speed of sound 
  double echoWidth = getEchoWidth();  // get duration of pulse returned by ultrasonic sensor 
  double echoDistance = getEchoDistance(echoWidth, speedOfSound);  // convert the duration of the pulse returned by the ultrasonic sensor to a distance 
  printInformation(angle, temperature, speedOfSound, echoWidth, echoDistance); // print the information to Serial 
    
  delay(sweepSpeed); 

}

//trigger the sensor
void triggerSensor(){
  digitalWrite(triggerPort, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPort, HIGH);
  delayMicroseconds(10); //10µs delay
  digitalWrite(triggerPort, LOW);
  delayMicroseconds(5);
}
// get the temperature from the LM35
double getTemperature(){
  int sensorValue = analogRead(temperaturePort);
  double mV = sensorValue * (5000.0 / 1023.0); // get off the pin 
  double temp = mV / 10; // convert voltage to celsius 
  return temp;
}

//returns speed of sound in cm/µs
//temperature - in °C
double getSpeedOfSound(double temperature){
  return (331.5 + 0.6 * temperature)*pow(10,-6)*pow(10,2); //formula provided in class
}

//returns width of echo in µs
double getEchoWidth(){
    return pulseIn(echoPort, HIGH);
}

//returns distance of echo in cm
//echoWidth - width of echo in µs
//speedOfSound - speed of sound in cm/µs
double getEchoDistance(double echoWidth, double speedOfSound){
  return echoWidth/(1/speedOfSound*2); //Formula given in class
}

//print distance, angle, and temperature to Serial 
void printInformation(double angle, double temperature, double speedOfSound, double echoWidth, double echoDistance){
      
    Serial.print(echoDistance);
    Serial.print(" ");
    
    Serial.print(angle);
    Serial.print(" ");
    
    Serial.print(temperature);
    Serial.println("");
}
