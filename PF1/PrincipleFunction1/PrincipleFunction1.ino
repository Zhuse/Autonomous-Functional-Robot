#include <Servo.h>
#include <LiquidCrystal.h>

/* Member variables */
float currTemp;
float currDist;
float speedSound; //Speed of sound based on current temp
float currSpeed = 0;
int servoPos = 90; // variable to store the servo position (90 deg is middle position)

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

double leftLastMillis;
double rightLastMillis;

double distToCenter = 3;

const int LCD_RS = 0;
const int LCD_EN = 1;
const int LCD_D4 = 2;
const int LCD_D5 = 3;
const int LCD_D6 = 4;
const int LCD_D7 = 5;

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

  attachInterrupt(digitalPinToInterrupt(LEFT_HE_PIN), updateLeftHE, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_HE_PIN), updateRightHE, RISING);

  lcd.begin(16, 2); //Setup LCD num of cols and rows
  updateLCD(1);
  
  // Set initial rotation speed to 0
  analogWrite(MOTOR_POWER_PIN1, 0);
  //digitalWrite(MOTOR_POLARITY_PIN1, LOW);
  analogWrite(MOTOR_POWER_PIN2, 0);
  //digitalWrite(MOTOR_POLARITY_PIN2, HIGH);

  //Put delay to start? 
}

void loop() {
  /*
  myservo.write(0);
  delay(2500);
  myservo.write(180);
  delay(2500);*/

  principleFunction1();
  
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
 * Initiate autonomous procedure for principle function 1
 * Robot moves forward at max speed until it detects an object within 50 cm
 * Robot gradually slows down and stops before hitting the wall (as close as pos.)
 * Robot checks left and right using servo and then chooses side with most space and repeats
 */
void principleFunction1(){
  while (getDist() > 50){
    //Max speed, set currSpeed
    Serial.print("Dist:");
    Serial.println(currDist);
    setForwardSpeed(255);
  }
  while (getDist() > 15) {
    Serial.print("Dist:");
    Serial.println(currDist);
    
    //Change speed as a function of distance
    setForwardSpeed(255-getDist()*255/15);
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
    stationaryTurn(1);
    //Turn left 90 degrees
  }
  else{
    Serial.println("RIGHT IS CLEAR");
    stationaryTurn(0);
    //Turn right 90 degrees
  }
}

/**
 * Updates the LCD display reading given the current speed of the robot (in the member variable)
 * and the mode it's in
 */
void updateLCD(int mode){
  lcd.clear();
  lcd.print("MODE: ");
  lcd.setCursor(6, 0);
  lcd.print(mode);
  lcd.setCursor(0, 1);
  lcd.print("SPEED: ");
  lcd.setCursor(7, 1);
  lcd.print(currSpeed);
}

/**
 * Turns motors such that both wheels go in forward direction
 * Changes currspeed to speed that we set forward speed to
 * Speed should be an int between 0 and 255
 */
void setForwardSpeed(int speed){
  digitalWrite(MOTOR_POLARITY_PIN1, LOW);
  digitalWrite(MOTOR_POLARITY_PIN2, HIGH);
  analogWrite(MOTOR_POWER_PIN1, speed);
  analogWrite(MOTOR_POWER_PIN2, speed);
  currSpeed = speed;
  updateLCD(1);
}

/**
 * Turns motors such that the whole robot turns in a direction without moving forward
 * Changes currspeed to 0
 * Direction is 1 for left and 0 for right
 */
void stationaryTurn(int direction){
  if (direction == 1){
    digitalWrite(MOTOR_POLARITY_PIN1, LOW);
    digitalWrite(MOTOR_POLARITY_PIN2, LOW);
    analogWrite(MOTOR_POWER_PIN1, 100);
    analogWrite(MOTOR_POWER_PIN2, 100);
    delay(5000);
  }
  else{
    digitalWrite(MOTOR_POLARITY_PIN1, LOW);
    digitalWrite(MOTOR_POLARITY_PIN2, LOW);
    analogWrite(MOTOR_POWER_PIN1, 100);
    analogWrite(MOTOR_POWER_PIN2, 100);
    delay(5000); 
  }
  analogWrite(MOTOR_POWER_PIN1, 0);
  analogWrite(MOTOR_POWER_PIN2, 0);
  currSpeed = 0;
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
}

void updateRightHE() {
  double timeChange = millis() - rightLastMillis;
  rightLastMillis = millis();
  Serial.println(calcTireSpeed(timeChange));
}

double calcTireSpeed(double time) {
  return (distToCenter * PI) / (time/1000); 
}

