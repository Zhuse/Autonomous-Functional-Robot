#include <Servo.h>
#include <LiquidCrystal.h>

/* Member variables */
float currTemp;
float currDist = 400;
float speedSound; //Speed of sound based on current temp
float currSpeed = 0;
float leftTireSpeed = 0;
float rightTireSpeed = 0;
int servoPos = 90; // variable to store the servo position (90 deg is middle position)
unsigned long timer = 0;

/* PF2 stuff */
int opticalSensors[] = {0,0,0,0};
int THRESHOLD = 500;

/* Constant Pin Assignments */
const int LM35_PIN = A0;
const int SERVO_PIN = A1;
const int HC_SR04_TRIG_PIN = 12;
const int HC_SR04_ECHO_PIN = 13;

const int MOTOR_POWER_PIN1 = 5; //E1 (left wheel speed)
const int MOTOR_POLARITY_PIN1 = 4; //M1 (left wheel) , LOW is forward
const int MOTOR_POWER_PIN2 = 6; //E1 (right wheel speed)
const int MOTOR_POLARITY_PIN2 = 7; //M1 (right wheel), HIGH is forward

//Pins for Hall Effect
const int RIGHT_HE_PIN = 2;
const int LEFT_HE_PIN = 3;

const int OPTICAL_SENSOR_PIN0 = A2;
const int OPTICAL_SENSOR_PIN1 = A3;

double leftLastMillis;
double rightLastMillis;

double distToCenter = 3;

const int LCD_RS = 0;
const int LCD_EN = 1;
const int LCD_D4 = 2;
const int LCD_D5 = 3;
const int LCD_D6 = 4;
const int LCD_D7 = 5;

int up = 0;
int down = 0;
int left = 0;
int right = 0;
int gear = 1;

Servo myservo;  // create servo object to control a servo
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7); //setup lcd

void setup() {
  Serial.begin(9600);
  pinMode(LM35_PIN, INPUT);
  pinMode(HC_SR04_TRIG_PIN, OUTPUT);
  pinMode(HC_SR04_ECHO_PIN, INPUT); 
  myservo.attach(SERVO_PIN);
  myservo.write(servoPos); //set servo position to mid
  pinMode(MOTOR_POLARITY_PIN1, OUTPUT); //Direction
  pinMode(MOTOR_POLARITY_PIN2, OUTPUT); //Direction
  
  pinMode(RIGHT_HE_PIN, INPUT_PULLUP);
  pinMode(LEFT_HE_PIN, INPUT_PULLUP);

  pinMode(OPTICAL_SENSOR_PIN0, INPUT);
  pinMode(OPTICAL_SENSOR_PIN1, INPUT);

  attachInterrupt(digitalPinToInterrupt(LEFT_HE_PIN), updateLeftHE, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_HE_PIN), updateRightHE, RISING);
  
  // Set initial rotation speed to 0
  analogWrite(MOTOR_POWER_PIN1, 0);
  //digitalWrite(MOTOR_POLARITY_PIN1, LOW);
  analogWrite(MOTOR_POWER_PIN2, 0);
  //digitalWrite(MOTOR_POLARITY_PIN2, HIGH);
}

void loop() {
  principleFunction3();
}

/**
 * Initiate autonomous procedure for principle function 1
 * Robot moves forward at max speed until it detects an object within 50 cm
 * Robot gradually slows down and stops before hitting the wall (as close as pos.)
 * Robot checks left and right using servo and then chooses side with most space and repeats
 */
void principleFunction1(){
  while (getDist()>10){
    if (currDist > 50){
      setForwardSpeed(255);
    }
    else if (currDist > 10) {
      //Change speed as a function of distance
      int forwardSpeed = 255.0 - currDist*25.5;
      setForwardSpeed(forwardSpeed);
    }
  }
  myservo.write(180); //Check left
  delay(500);
  float leftDist = getDist();
  myservo.write(0); //Check right
  delay(500);
  float rightDist = getDist();
  myservo.write(90); //Reset position

  if (leftDist>rightDist){
    Serial.println("LEFT IS CLEAR");
    stationaryLeftTurn();
    //Turn left 90 degrees
  }
  else{
    Serial.println("RIGHT IS CLEAR");
    stationaryRightTurn();
    //Turn right 90 degrees
  }
}

void principleFunction3(){
  String command = getProcessingCommand();
  switch (1) {
  case 1://"forwardStraight":
    setForwardSpeed(255);
    break;
  case 2://"forwardRight":
    movingRightTurn(255);
    break;
  case 3://"forwardLeft":
    movingLeftTurn(255);
    break;
  case 4://"backwardsStraight":
    setForwardSpeed(-255);
    break;
  case 5://backwardsLeft
    movingLeftTurn(-255);
    break;
  case 6://backwardsRight
    movingRightTurn(-255);
    break;
  default: //stop
    stopRobot();
}
  
}


void getProcessingCommand(){
  int instruction = 0;
  while (true) {
      if (Serial.available()>0) {
        instruction = (int)Serial.read();
        up = instruction / 10000;
        down = instruction / 1000 % 10;
        left = instruction / 100 % 10;
        right = instruction / 10 % 10;
        gear = instruction % 10;
      }
  }
}


/**
 * Turns motors such that both wheels go in forward direction
 * Changes currspeed to speed that we set forward speed to
 * Speed should be an int between -255 and 255
 */
void setForwardSpeed(int speed){
  int wheel_pin1, wheel_pin2;
  if (speed>0){
    wheel_pin1 = MOTOR_POLARITY_PIN1;
    wheel_pin2 = MOTOR_POLARITY_PIN2;
  }
  else{
    wheel_pin1 = MOTOR_POLARITY_PIN2;
    wheel_pin2 = MOTOR_POLARITY_PIN1;
  }
  digitalWrite(wheel_pin1, LOW);
  digitalWrite(wheel_pin2, HIGH);
  currSpeed = speed;
  speed = abs(speed);

  //Calibrate:
  analogWrite(MOTOR_POWER_PIN1, speed);
  analogWrite(MOTOR_POWER_PIN2, speed);
  while (leftTireSpeed<rightTireSpeed){
    analogWrite(MOTOR_POWER_PIN2, speed-=5);
    delay(50);
  }
  while (rightTireSpeed<leftTireSpeed){
    analogWrite(MOTOR_POWER_PIN1, speed-=5);
    delay(50);
  }
  
  //updateLCD();
}

/*
 * Turns arduino left while setting right motor speed to speed
 */
void movingLeftTurn(int speed){
  int wheel_pin1, wheel_pin2;
  if (speed>0){
    wheel_pin1 = MOTOR_POLARITY_PIN1;
    wheel_pin2 = MOTOR_POLARITY_PIN2;
  }
  else{
    wheel_pin1 = MOTOR_POLARITY_PIN2;
    wheel_pin2 = MOTOR_POLARITY_PIN1;
  }
  digitalWrite(wheel_pin1, LOW);
  digitalWrite(wheel_pin2, HIGH);
  currSpeed = speed;
  speed = abs(speed);

  //May need calibration
  analogWrite(MOTOR_POWER_PIN1, speed);
  if (speed-100<0)
      analogWrite(MOTOR_POWER_PIN2, 0);
  else
    analogWrite(MOTOR_POWER_PIN2, speed-100);
}

/*
 * Turns arduino right while setting left motor speed to speed
 */
void movingRightTurn(int speed){
  int wheel_pin1, wheel_pin2;
  if (speed>0){
    wheel_pin1 = MOTOR_POLARITY_PIN1;
    wheel_pin2 = MOTOR_POLARITY_PIN2;
  }
  else{
    wheel_pin1 = MOTOR_POLARITY_PIN2;
    wheel_pin2 = MOTOR_POLARITY_PIN1;
  }
  digitalWrite(wheel_pin1, LOW);
  digitalWrite(wheel_pin2, HIGH);
  currSpeed = speed;
  speed = abs(speed);
  
  //May need calibration
  analogWrite(MOTOR_POWER_PIN2, speed);
  if (speed-100<0)
      analogWrite(MOTOR_POWER_PIN1, 0);
  else
    analogWrite(MOTOR_POWER_PIN1, speed-100);
}

/**
 * Stops movement in the robot
 */
void stopRobot(){
  analogWrite(MOTOR_POWER_PIN1, 0);
  analogWrite(MOTOR_POWER_PIN2, 0);
  currSpeed = 0;
}

/**
 * Turns motors such that the whole robot turns left without moving forward
 * Changes currspeed to 0
 */
void stationaryLeftTurn(){
  currSpeed = 0;
  //updateLCD();
  
  digitalWrite(MOTOR_POLARITY_PIN1, LOW);
  digitalWrite(MOTOR_POLARITY_PIN2, LOW);
  analogWrite(MOTOR_POWER_PIN1, 255); //Left wheel
  analogWrite(MOTOR_POWER_PIN2, 255); //Right wheel
  delay(5000);
  
  analogWrite(MOTOR_POWER_PIN1, 0);
  analogWrite(MOTOR_POWER_PIN2, 0);
}

/**
 * Turns motors such that the whole robot turns right without moving forward
 * Changes currspeed to 0
 */
void stationaryRightTurn(){
  currSpeed = 0;
  //updateLCD();
  
  digitalWrite(MOTOR_POLARITY_PIN1, LOW);
  digitalWrite(MOTOR_POLARITY_PIN2, LOW);
  analogWrite(MOTOR_POWER_PIN1, 255);
  analogWrite(MOTOR_POWER_PIN2, 255);
  delay(5000); 
    
  analogWrite(MOTOR_POWER_PIN1, 0);
  analogWrite(MOTOR_POWER_PIN2, 0);
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

void updateLeftHE() {
  double timeChange = millis() - leftLastMillis;
  leftLastMillis = millis();
  Serial.println(calcTireSpeed(timeChange));
  leftTireSpeed = calcTireSpeed(timeChange);
}

void updateRightHE() {
  double timeChange = millis() - rightLastMillis;
  rightLastMillis = millis();
  Serial.println(calcTireSpeed(timeChange));
  rightTireSpeed = calcTireSpeed(timeChange);
}

double calcTireSpeed(double time) {
  return (distToCenter * PI) / (time/1000); 
}

