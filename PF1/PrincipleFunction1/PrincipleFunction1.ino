#include <Servo.h>

Servo myservo;  // create servo object to control a servo

/* Member variables */
float currTemp;
float currDist;
float speedSound; //Speed of sound based on current temp
float currSpeed;
int servoPos = 90;    // variable to store the servo position (90 deg is middle position)

/* Constants */
const int LM35_PIN = 0;
const int SERVO_PIN = 13;
const int HC_SR04_TRIG_PIN = 10;
const int HC_SR04_ECHO_PIN = 9;

void setup() {
  Serial.begin(9600);
  pinMode(LM35_PIN, INPUT);
  pinMode(HC_SR04_TRIG_PIN, OUTPUT);
  pinMode(HC_SR04_ECHO_PIN, INPUT); 
  myservo.attach(SERVO_PIN);
  myservo.write(servoPos); //set servo position to mid
}

void loop() {


}

/**
 * Initiate autonomous procedure for principle function 1
 * Robot moves forward at max speed until it detects an object within 50 cm
 * Robot gradually slows down and stops before hitting the wall (as close as pos.)
 * Robot checks left and right using servo and then chooses side with most space and repeats
 */
void principleFunction1(){
  while (getDist() > 50){
    //Max speed, set currSpeed
  }
  while (currSpeed > 0) {
    //Change speed as a function of distance
  }
  myservo.write(0);
  float leftDist = getDist();
  myservo.write(180);
  float rightDist = getDist();

  if (leftDist>rightDist){
    //Turn left 90 degrees
  }
  else{
    //Turn right 90 degrees
  }
}

/**
 * Returns the distance reading from the HC SR04 in cm
 * Also modifies member variables: currTemp and speedSound and currDist
 */
float getDist(){
  currTemp = getLMTemp(LM35_PIN);
  speedSound = 331.5 + (0.6 * currTemp);
  currDist = readHCSR04(HC_SR04_TRIG_PIN, HC_SR04_ECHO_PIN);
  return currDist;
}

/**
 * Get the temperature reading from the LM35 (in degrees celsius)
 */
float getLMTemp(int PIN){
  return (5.0 * analogRead(PIN) * 100.0 / 1024);
}

/**
 * Initiates and takes a reading from the HC-SR04.
 * Returns a distance to detected object in cm.
 * If no object is detected returns 400 (max range of device)
 */
float readHCSR04(int trigPin, int echoPin){
  initiateHCSR04(trigPin);
  return receiveHCSR04(echoPin);
}

/**
 * Procedure to initiate HC SR04 sensor reading
 */
void initiateHCSR04(int trigPin){  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //Pulse high for at least 10us

  digitalWrite(trigPin, LOW); //End pulse
}

/**
 * Procedure to receive response from HC SR04 sensor reading and return distance in cm
 * Will return -1 if no obstacle is detected
 */
float receiveHCSR04(int echoPin){
  unsigned long pulseDuration;
  unsigned long timeOutDuration;

  timeOutDuration = 36000;
  pulseDuration = pulseIn(echoPin, HIGH, timeOutDuration);

  if (pulseDuration == 0){ //If echo pulse times out, no obstacle is detected
    return 400;
  }
  else {
    return pulseDuration/(20000.0/speedSound); //Return distance otherwise
  }
}
