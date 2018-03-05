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
int opticalSensors[] = {0, 0, 0, 0};
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

//Pins for DIP Switch //TODO CHANGE THIS
const int DIP_1 = 1;
const int DIP_2 = 2;
const int DIP_3 = 3;
int mode;  //decides which principle function to run on setup(). 0 for PF1, 1 for PF2, 2 for AF
const int MODE_PF1 = 0;
const int MODE_PF2 = 1;
const int MODE_AF = 2;

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

  lcd.begin(16, 2); //Setup LCD num of cols and rows
  updateLCD();

  // Set initial rotation speed to 0
  analogWrite(MOTOR_POWER_PIN1, 0);
  //digitalWrite(MOTOR_POLARITY_PIN1, LOW);
  analogWrite(MOTOR_POWER_PIN2, 0);
  //digitalWrite(MOTOR_POLARITY_PIN2, HIGH);

  //check dip switch status and set mode appropriately. Only DIP_1 on for PF1, Only DIP_2 on for PF2, Only DIP_3 on for AF
  //error check for unsupported dip switch input
  while (digitalRead(DIP_1) && digitalRead(DIP_2) && digitalRead(DIP_3)
         || digitalRead(DIP_1) && digitalRead(DIP_2)
         || digitalRead(DIP_2) && digitalRead(DIP_3)
         || digitalRead(DIP_1) && digitalRead(DIP_3)) {
    //TODO print error message to LCD

    if(digitalRead(DIP_1)){
      mode = MODE_PF1;
    }
    else if(digitalRead(DIP_2)){
      mode = MODE_PF2;
    }
    else{
      mode = MODE_AF;
    }
//Put delay to start?
}

void loop() {
  /*
  myservo.write(0);
  delay(2500);
  myservo.write(180);
  delay(2500);*/
  switch(mode){
    case MODE_PF1: principleFunction1();
    case MODE_PF2: principleFunction2();
    case MODE_AF: additionalFunctionality();
  }  
  /*
    for (int i=0; i<200; i+=5){
    setForwardSpeed(i);
    Serial.println(getLMTemp(LM35_PIN));
    if (i<100){
      myservo.write(0);
    }
    else{
      myservo.write(180);
    }
    delay(250);
    }*/
}

/**
   Initiate autonomous procedure for principle function 1
   Robot moves forward at max speed until it detects an object within 50 cm
   Robot gradually slows down and stops before hitting the wall (as close as pos.)
   Robot checks left and right using servo and then chooses side with most space and repeats
*/
void principleFunction1() {
  while (getDist() > 10) {
    if (currDist > 50) {
      //Max speed, set currSpeed
      //Serial.print("Dist:");
      //Serial.println(currDist);
      setForwardSpeed(255);
    }
    else if (currDist > 10) {
      //Serial.print("Dist:");
      //Serial.println(currDist);

      //Change speed as a function of distance
      int forwardSpeed = 255.0 - currDist * 25.5;
      Serial.print("SPEED: ");
      Serial.println(forwardSpeed);
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

  if (leftDist > rightDist) {
    Serial.println("LEFT IS CLEAR");
    stationaryLeftTurn();
    //Turn left 90 degrees
  }
  else {
    Serial.println("RIGHT IS CLEAR");
    stationaryRightTurn();
    //Turn right 90 degrees
  }
}

/*
   Principle function 2 allows robot to track and follow black electrical tape laid on the ground
*/
void principleFunction2() {
  updateOpticalSensors();
  updateDrive();
}

/*
 * additional functionality allows user to control robot remotely via bluetooth using keyboard input or hand tracking
 */
void additionalFunctionality(){
  
}

/*
 * Updates PF2 motor speeds depending on current optical sensor readings
 */
void updateDrive() {
  if (!opticalSensors[0] && !opticalSensors[1]) {
    setForwardSpeed(255);
  } else if (!opticalSensors[0] && opticalSensors[1]) {
    // off to the right
    movingRightTurn(255);
  } else if (opticalSensors[0] && !opticalSensors[1]) {
    movingLeftTurn(255);
  } else if (opticalSensors[0] && opticalSensors[1]) {
    setForwardSpeed(0);
  }
}

/*
   Reads the 4 optical sensors and updates sensors array
*/
void updateOpticalSensors() {

  if (analogRead(OPTICAL_SENSOR_PIN0) > THRESHOLD) {
    opticalSensors[0] = 1;
  } else {
    opticalSensors[0] = 0;
  }

  if (analogRead(OPTICAL_SENSOR_PIN1) > THRESHOLD) {
    opticalSensors[1] = 1;
  } else {
    opticalSensors[1] = 0;
  }
}

/**
   Updates the LCD display reading given the current speed of the robot (in the member variable)
   and the mode it's in
*/
void updateLCD() {
  if (millis() - timer < 100)
    return; //Do not update more than once a second

  timer = millis();
  lcd.clear();
  lcd.print("DIST: ");
  lcd.setCursor(6, 0);
  lcd.print(currDist);
  lcd.setCursor(0, 1);
  lcd.print("SPEED: ");
  lcd.setCursor(7, 1);
  lcd.print(currSpeed);
}

/**
   Turns motors such that both wheels go in forward direction
   Changes currspeed to speed that we set forward speed to
   Speed should be an int between 0 and 255
*/
void setForwardSpeed(int speed) {
  if (speed < 0) {
    speed = 0;
  }
  digitalWrite(MOTOR_POLARITY_PIN1, LOW);
  digitalWrite(MOTOR_POLARITY_PIN2, HIGH);

  //Calibrate:
  analogWrite(MOTOR_POWER_PIN1, speed);
  analogWrite(MOTOR_POWER_PIN2, speed);
  while (leftTireSpeed < rightTireSpeed) {
    analogWrite(MOTOR_POWER_PIN2, speed -= 5);
    delay(50);
  }
  while (rightTireSpeed < leftTireSpeed) {
    analogWrite(MOTOR_POWER_PIN1, speed -= 5);
    delay(50);
  }

  currSpeed = speed;
  //updateLCD();
}

/*
   Turns arduino left while setting right motor speed to speed
*/
void movingLeftTurn(int speed) {
  if (speed < 0) {
    speed = 0;
  }
  digitalWrite(MOTOR_POLARITY_PIN1, LOW);
  digitalWrite(MOTOR_POLARITY_PIN2, HIGH);

  //May need calibration
  analogWrite(MOTOR_POWER_PIN1, speed);
  if (speed - 100 < 0)
    analogWrite(MOTOR_POWER_PIN2, 0);
  else
    analogWrite(MOTOR_POWER_PIN2, speed - 100);
  currSpeed = speed;
}

/*
   Turns arduino right while setting left motor speed to speed
*/
void movingRightTurn(int speed) {
  if (speed < 0) {
    speed = 0;
  }
  digitalWrite(MOTOR_POLARITY_PIN1, LOW);
  digitalWrite(MOTOR_POLARITY_PIN2, HIGH);

  //May need calibration
  analogWrite(MOTOR_POWER_PIN2, speed);
  if (speed - 100 < 0)
    analogWrite(MOTOR_POWER_PIN1, 0);
  else
    analogWrite(MOTOR_POWER_PIN1, speed - 100);
  currSpeed = speed;
}

/**
   Turns motors such that the whole robot turns left without moving forward
   Changes currspeed to 0
*/
void stationaryLeftTurn() {
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
   Turns motors such that the whole robot turns right without moving forward
   Changes currspeed to 0
*/
void stationaryRightTurn() {
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
   Returns the distance reading from the HC SR04 in cm
   Also modifies member variables: currTemp and speedSound and currDist
*/
float getDist() {
  currTemp = getLMTemp(LM35_PIN);
  speedSound = 331.5 + (0.6 * currTemp);
  currDist = readHCSR04(HC_SR04_TRIG_PIN, HC_SR04_ECHO_PIN);
  return currDist;
}

/**
   Get the temperature reading from the LM35 (in degrees celsius)
*/
float getLMTemp(int PIN) {
  return (5.0 * analogRead(PIN) * 100.0 / 1024);
}

/**
   Initiates and takes a reading from the HC-SR04.
   Returns a distance to detected object in cm.
   If no object is detected returns 400 (max range of device)
*/
float readHCSR04(int trigPin, int echoPin) {
  initiateHCSR04(trigPin);
  return receiveHCSR04(echoPin);
}

/**
   Procedure to initiate HC SR04 sensor reading
*/
void initiateHCSR04(int trigPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //Pulse high for at least 10us

  digitalWrite(trigPin, LOW); //End pulse
}

/**
   Procedure to receive response from HC SR04 sensor reading and return distance in cm
   Will return -1 if no obstacle is detected
*/
float receiveHCSR04(int echoPin) {
  unsigned long pulseDuration;
  unsigned long timeOutDuration;

  timeOutDuration = 36000;
  pulseDuration = pulseIn(echoPin, HIGH, timeOutDuration);

  if (pulseDuration == 0) { //If echo pulse times out, no obstacle is detected
    return 400;
  }
  else {
    return pulseDuration / (20000.0 / speedSound); //Return distance otherwise
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
  return (distToCenter * PI) / (time / 1000);
}

