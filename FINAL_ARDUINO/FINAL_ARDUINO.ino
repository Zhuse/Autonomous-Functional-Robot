#include <Servo.h>
#include <SoftwareSerial.h>

/* Member variables */
float currTemp;
float currDist;
float speedSound; //Speed of sound based on current temp
float currSpeed = 0;
float leftTireSpeed = 0;
float rightTireSpeed = 0;
int servoPos = 90; // variable to store the servo position (90 deg is middle position)
unsigned long lcdTimer = 0;
unsigned long lcdResetTimer = 0;

/* PF2 stuff */
int opticalSensors[] = {0, 0, 0, 0};
int THRESHOLD = 500;
int lastAdjustment = 0; //0 for left 1 for right
int delayCount = 0;

/* PF3 stuff*/
int up = 0;
int down = 0;
int left = 0;
int right = 0;
int gear = 1;

/* Constant Pin Assignments */
const int LM35_PIN = A0;
const int SERVO_PIN = A1;
const int HC_SR04_TRIG_PIN = 12;
const int HC_SR04_ECHO_PIN = 13;

const int MOTOR_POWER_PIN_LEFT = 5; //E1 (left wheel speed)
const int MOTOR_POLARITY_PIN_LEFT = 4; //M1 (left wheel) , LOW is forward
const int MOTOR_POWER_PIN_RIGHT = 6; //E1 (right wheel speed)
const int MOTOR_POLARITY_PIN_RIGHT = 7; //M1 (right wheel), HIGH is forward

const int OPTICAL_SENSOR_PIN0 = A2;
const int OPTICAL_SENSOR_PIN1 = A3;

const int DATA_PIN = 11;
const int LATCH_PIN = 10;
const int CLOCK_PIN = 9;

//Pins for DIP Switch
const int DIP_PIN1 = A4;
const int DIP_PIN2 = A5;

//Pins for Hall Effect
const int RIGHT_HE_PIN = 3;
const int LEFT_HE_PIN = 2;

const int RX_PIN = 0;
const int TX_PIN = 1;

const int MAX_SPEED = 255; //Max motor speed

/*Characters encodings for LCD*/
const byte LCD_0 = B00110000;
const byte LCD_1 = B00110001;
const byte LCD_2 = B00110010;
const byte LCD_3 = B00110011;
const byte LCD_4 = B00110100;
const byte LCD_5 = B00110101;
const byte LCD_6 = B00110110;
const byte LCD_7 = B00110111;
const byte LCD_8 = B00111000;
const byte LCD_9 = B00111001;
const byte LCD_A = B01000001;
const byte LCD_B = B01000010;
const byte LCD_C = B01000011;
const byte LCD_D = B01000100;
const byte LCD_E = B01000101;
const byte LCD_F = B01000110;
const byte LCD_G = B01000111;
const byte LCD_H = B01001000;
const byte LCD_I = B01001001;
const byte LCD_J = B01001010;
const byte LCD_K = B01001011;
const byte LCD_L = B01001100;
const byte LCD_M = B01001101;
const byte LCD_N = B01001110;
const byte LCD_O = B01001111;
const byte LCD_P = B01010000;
const byte LCD_Q = B01010001;
const byte LCD_R = B01010010;
const byte LCD_S = B01010011;
const byte LCD_T = B01010100;
const byte LCD_U = B01010101;
const byte LCD_V = B01010110;
const byte LCD_W = B01010111;
const byte LCD_X = B01011000;
const byte LCD_Y = B01011001;
const byte LCD_Z = B01011010;
const byte LCD_SPACE = B00100000;
const byte LCD_DASH = B00101101;
const byte LCD_EXCLMARK = B00100001;
const byte LCD_COLON = B00111010;

/*Hall effect */
double leftLastMillis;
double rightLastMillis;
int rightRPM = 0;
int leftRPM = 0;
double distToCenter = 3;

const int LEFT_FWD = HIGH;
const int LEFT_BWD = LOW;
const int RIGHT_FWD = HIGH;
const int RIGHT_BWD = LOW;

Servo myservo;  // create servo object to control a servo
SoftwareSerial BT(TX_PIN, RX_PIN);

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  pinMode(LM35_PIN, INPUT);
  pinMode(HC_SR04_TRIG_PIN, OUTPUT);
  pinMode(HC_SR04_ECHO_PIN, INPUT);
  myservo.attach(SERVO_PIN);
  myservo.write(servoPos); //set servo position to mid
  pinMode(MOTOR_POLARITY_PIN_LEFT, OUTPUT); //Direction
  pinMode(MOTOR_POLARITY_PIN_RIGHT, OUTPUT); //Direction
  pinMode(DIP_PIN1, INPUT);
  pinMode(DIP_PIN2, INPUT);

  pinMode(RIGHT_HE_PIN, INPUT_PULLUP);
  pinMode(LEFT_HE_PIN, INPUT_PULLUP);

  pinMode(OPTICAL_SENSOR_PIN0, INPUT);
  pinMode(OPTICAL_SENSOR_PIN1, INPUT);

  //For shift reg for LCD
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(LEFT_HE_PIN), updateLeftHE, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_HE_PIN), updateRightHE, RISING);

  // Set initial rotation speed to 0
  analogWrite(MOTOR_POWER_PIN_LEFT, 0);
  analogWrite(MOTOR_POWER_PIN_RIGHT, 0);

  initializeLCD();
}

void loop() {
  //Read DIP pins after each loop of some PF
  if (!digitalRead(DIP_PIN1) && !digitalRead(DIP_PIN2)) {
    //resetLCD();
    updateLCDMode(0);
    stopRobot();
  }
  else if (digitalRead(DIP_PIN1) && !digitalRead(DIP_PIN2)) {
    //resetLCD();
    updateLCDMode(2);
    principleFunction2();
  }
  else if (!digitalRead(DIP_PIN1) && digitalRead(DIP_PIN2)) {
    //resetLCD();
    updateLCDMode(1);
    principleFunction1();
  }
  else {
    //resetLCD();
    updateLCDMode(3);
    stopRobot();
    //principleFunction3();
  }
}

/**
   Initiate autonomous procedure for principle function 1
   Robot moves forward at max speed until it detects an object within 50 cm
   Robot gradually slows down and stops before hitting the wall (as close as pos.)
   Robot checks left and right using servo and then chooses side with most space and repeats
*/
void principleFunction1() {
  while (getDist() > 5) {
    if (currDist > 25) {
      //Max speed, set currSpeed
      //Serial.print("Dist:");
      //Serial.println(currDist);
      setForwardSpeed(MAX_SPEED);
    }
    else {
      //Serial.print("Dist:");
      //Serial.println(currDist);

      //Change speed as a function of distance
      //int forwardSpeed = MAX_SPEED.0 - currDist * 25.5;
      //Serial.print("SPEED: ");
      //Serial.println(forwardSpeed);
      setForwardSpeed(150);
    }
  }
  setForwardSpeed(0);
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
    delay(35);
    //Turn left 90 degrees
  }
  else {
    Serial.println("RIGHT IS CLEAR");
    stationaryRightTurn();
    delay(35);
    //Turn right 90 degrees
  }
  stopRobot();
}

/*
   Principle function 2 allows robot to track and follow black electrical tape laid on the ground
*/
void principleFunction2() {
  updateOpticalSensors();
  updateDrive();
}

/*
   Principle function 3 allows arduino to communicate through bluetooth with a computer (using Processing)
   The user can control the robot using traditional arrow key controls or through hand gestures using
   a Leap Motion hand gesture device
*/
void principleFunction3() {
  getProcessingCommand(); //Updates movement member variables
  if (up && left) {
    movingLeftTurn(255);
  }
  else if (up && right) {
    movingRightTurn(255);
  }
  else if (down && left) {
    movingLeftTurn(-255);
  }
  else if (down && right) {
    movingRightTurn(-255);
  }
  else if (up) {
    setForwardSpeed(255);
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13, LOW);
  }
  else if (down) {
    setForwardSpeed(-255);
  }
  else if (left) {
    stationaryLeftTurn();
    digitalWrite(13, HIGH);
    delay(666);
    digitalWrite(13, LOW);
  }
  else if (right) {
    stationaryRightTurn();
    digitalWrite(13, HIGH);
    delay(1250);
    digitalWrite(13, LOW);
  }
  else {
    stopRobot();
  }
}

/*
   Gets instruction code from processing for PF3
*/
void getProcessingCommand() {
  String command = "";
  if (BT.available()) {
    while (BT.available()) { // While there is more to be read, keep reading.
      command += (char)BT.read();
    }
  }
  int instruction = command.toInt();
  while (instruction >= 100000) { //Reduce instruction to 5 digits
    instruction /= 10;
  }
  switch (instruction) {
    case 0:
      BT.println("OFF");
      break;
    case 16:
      BT.println("UP");
      break;
    case 242:
      BT.println("DOWN-RIGHT");
      break;
    case 100:
      BT.println("LEFT");
      break;
    case 10:
      BT.println("RIGHT");
      break;
    case 232:
      BT.println("DOWN");
      break;
    case 76:
      BT.println("DOWN-LEFT");
      break;
    case 116:
      BT.println("UP-LEFT");
      break;
    case 26:
      BT.println("UP-RIGHT");
      break;
    default: break;
  }
  //Alternate implementation in case above doesn't work
  //    String command = "";
  //  if (BT.available()) {
  //    while (BT.available()) { // While there is more to be read, keep reading.
  //      command += (char)BT.read();
  //    }
  //  }
  //    up = command.substring(0,1).toInt();
  //    down = command.substring(1,2).toInt();
  //    left = command.substring(2,3).toInt();
  //    right = command.substring(3,4).toInt();
  //    gear = command.substring(4,5).toInt();
}

/*
   Updates PF2 motor speeds depending on current optical sensor readings
*/
void updateDrive() {
  if (!opticalSensors[0] && !opticalSensors[1]) {
    setForwardSpeed(MAX_SPEED);
    delayCount = 0;
    lastAdjustment = 10;
    return;
  } else if (!opticalSensors[0] && opticalSensors[1]) {
    // off to the right
    delayCount = 0;
    lastAdjustment = 0;
    reduceLeft(1);
  } else if (opticalSensors[0] && !opticalSensors[1]) {
    lastAdjustment = 1;
    delayCount = 0;
    reduceRight(1);
  } else if (opticalSensors[0] && opticalSensors[1]) {
    if (delayCount < 100) {
      if (lastAdjustment == 0) {
        reduceLeft(1);
      } else {
        reduceRight(1);
      }
      delayCount++;
      return;
    }
    stopRobot();
  }
}

// reduce speed of left motor depending on value of spd
// if spd == 1 reduce a bit, if spd == 2 reduce a lot
void reduceLeft(int spd) {
  if (spd == 1) {
    analogWrite(MOTOR_POWER_PIN_LEFT, MAX_SPEED - 180);
  } else if (spd == 2) {
    analogWrite(MOTOR_POWER_PIN_LEFT, MAX_SPEED - 50);
  }
}

// reduce speed of right motor
// if spd == 1 reduce a bit, if spd == 2 reduce a lot
void reduceRight(int spd) {
  if (spd == 1) {
    analogWrite(MOTOR_POWER_PIN_RIGHT, MAX_SPEED - 180);
  } else if (spd == 2) {
    analogWrite(MOTOR_POWER_PIN_RIGHT, MAX_SPEED - 50);
  }
}

/*
   Reads the 4 optical sensors and updates sensors array
*/
void updateOpticalSensors() {
  if (analogRead(OPTICAL_SENSOR_PIN0) > THRESHOLD) {
    opticalSensors[0] = 0;
  } else {
    opticalSensors[0] = 1;
  }
  if (analogRead(OPTICAL_SENSOR_PIN1) > THRESHOLD) {
    opticalSensors[1] = 0;
  } else {
    opticalSensors[1] = 1;
  }
}

/**
   Turns motors such that both wheels go in forward direction
   Changes currspeed to speed that we set forward speed to
   Speed should be an int between -255 and 255 (MAX_SPEED)
*/
void setForwardSpeed(int speed) {
  if (speed >= 0) {
    digitalWrite(MOTOR_POLARITY_PIN_LEFT, LEFT_FWD);
    digitalWrite(MOTOR_POLARITY_PIN_RIGHT, RIGHT_FWD);
  }
  else {
    digitalWrite(MOTOR_POLARITY_PIN_LEFT, LEFT_BWD);
    digitalWrite(MOTOR_POLARITY_PIN_RIGHT, RIGHT_BWD);
  }
  currSpeed = speed; //Set member var to speed with sign
  speed = abs(speed);
  analogWrite(MOTOR_POWER_PIN_LEFT, speed);
  analogWrite(MOTOR_POWER_PIN_RIGHT, speed);
  //Calibrate with HE:
  /*
    if (leftTireSpeed + 100 < rightTireSpeed)
      while (leftTireSpeed < rightTireSpeed) {
        analogWrite(MOTOR_POWER_PIN_RIGHT, speed -= 1);
        delay(10);
      }
    else if (leftTireSpeed > rightTireSpeed + 100)
      while (rightTireSpeed < leftTireSpeed) {
        analogWrite(MOTOR_POWER_PIN_LEFT, speed -= 1);
        delay(10);
      }*/
}

/*
   Turns arduino left while setting right motor speed to speed
   Speed can be between -255 and 255
*/
void movingLeftTurn(int speed) {
  if (speed >= 0) {
    digitalWrite(MOTOR_POLARITY_PIN_LEFT, LEFT_FWD);
    digitalWrite(MOTOR_POLARITY_PIN_RIGHT, RIGHT_FWD);
  }
  else {
    digitalWrite(MOTOR_POLARITY_PIN_LEFT, LEFT_BWD);
    digitalWrite(MOTOR_POLARITY_PIN_RIGHT, RIGHT_BWD);
  }
  currSpeed = speed; //Set member var to speed with sign
  speed = abs(speed);
  analogWrite(MOTOR_POWER_PIN_LEFT, speed - 100);
  analogWrite(MOTOR_POWER_PIN_RIGHT, speed);
}

/*
   Turns arduino right while setting left motor speed to speed
   Speed can be between -255 and 255
*/
void movingRightTurn(int speed) {
  if (speed >= 0) {
    digitalWrite(MOTOR_POLARITY_PIN_LEFT, LEFT_FWD);
    digitalWrite(MOTOR_POLARITY_PIN_RIGHT, RIGHT_FWD);
  }
  else {
    digitalWrite(MOTOR_POLARITY_PIN_LEFT, LEFT_BWD);
    digitalWrite(MOTOR_POLARITY_PIN_RIGHT, RIGHT_BWD);
  }
  currSpeed = speed; //Set member var to speed with sign
  speed = abs(speed);
  analogWrite(MOTOR_POWER_PIN_LEFT, speed);
  analogWrite(MOTOR_POWER_PIN_RIGHT, speed - 100);
}

/**
   Turns motors such that the whole robot turns left without moving forward
   Changes currspeed to 0
*/
void stationaryLeftTurn() {
  currSpeed = 0;

  digitalWrite(MOTOR_POLARITY_PIN_LEFT, LEFT_BWD);
  digitalWrite(MOTOR_POLARITY_PIN_RIGHT, RIGHT_FWD);
  analogWrite(MOTOR_POWER_PIN_LEFT, MAX_SPEED); //Left wheel
  analogWrite(MOTOR_POWER_PIN_RIGHT, MAX_SPEED); //Right wheel
  /*
    delay(1000);
    analogWrite(MOTOR_POWER_PIN_LEFT, 0);
    analogWrite(MOTOR_POWER_PIN_RIGHT, 0);*/
}

/**
   Turns motors such that the whole robot turns right without moving forward
   Changes currspeed to 0
*/
void stationaryRightTurn() {
  currSpeed = 0;

  digitalWrite(MOTOR_POLARITY_PIN_LEFT, LEFT_FWD);
  digitalWrite(MOTOR_POLARITY_PIN_RIGHT, RIGHT_BWD);
  analogWrite(MOTOR_POWER_PIN_LEFT, MAX_SPEED);
  analogWrite(MOTOR_POWER_PIN_RIGHT, MAX_SPEED);
  /*
    delay(1000);
    analogWrite(MOTOR_POWER_PIN_LEFT, 0);
    analogWrite(MOTOR_POWER_PIN_RIGHT, 0);*/
}

/**
  Stops movement in the robot
*/
void stopRobot() {
  analogWrite(MOTOR_POWER_PIN_LEFT, 0);
  analogWrite(MOTOR_POWER_PIN_RIGHT, 0);
  currSpeed = 0;
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

/**
   Update left hall effect sensor value, triggered by interrupt
*/
void updateLeftHE() {
  leftRPM++;
  if (leftRPM > 3) {
    double timeChange = millis() - leftLastMillis;
    leftLastMillis = millis();
    Serial.print("LEFT HE ");
    Serial.println(calcTireSpeed(timeChange));
    leftTireSpeed = calcTireSpeed(timeChange);
    leftRPM = 0;
    updateLCDSpeed((leftTireSpeed + rightTireSpeed) / 2);
  }
}

/**
   Update right hall effect sensor value, triggered by interrupt
*/
void updateRightHE() {
  rightRPM++;
  if (rightRPM > 3) {
    double timeChange = millis() - rightLastMillis;
    rightLastMillis = millis();
    Serial.print("RIGHT HE ");
    Serial.println(calcTireSpeed(timeChange));
    rightTireSpeed = calcTireSpeed(timeChange);
    rightRPM = 0;
    updateLCDSpeed((leftTireSpeed + rightTireSpeed) / 2);
  }
}

/**
   Calculate tire speeds using hall effect sensors
*/
double calcTireSpeed(double time) {
  if (time > 1000) return 0;
  return (distToCenter * PI) / (time / 1000);
}

/*
   Update the LCD value for mode
*/
void updateLCDMode(int mode) {
  LCD_Write(B10000101, 0); //Set cursor to end of first line
  for (int i = 0; i < 10; i++) {
    LCD_Write(LCD_SPACE, 1);
  }
  writeDigitLCD(mode);
}

/*
   Update the LCD value speed
*/
void updateLCDSpeed(int speed) {
  if (millis() - lcdTimer > 250) {
    if (speed < 0) {
      LCD_Write(B11000110, 0); //Sets cursor to second line
      LCD_Write(LCD_DASH, 1);
      speed = abs(speed);
    }
    else {
      LCD_Write(B11000111, 0); //Sets cursor to second line
    }
    int speedDigits[9];
    for (int i = 0; i < 9; i++) {
      speedDigits[i] = speed % 10;
      speed /= 10;
    }
    for (int i = 8; i >= 0; i--) {
      writeDigitLCD(speedDigits[i]);
    }
    lcdTimer = millis();
  }
}

/*
   Write some digit to the LCD at the position the cursor is at
*/
void writeDigitLCD(int digit) {
  switch (digit) {
    case 1:
      LCD_Write(LCD_1, 1);
      break;
    case 2:
      LCD_Write(LCD_2, 1);
      break;
    case 3:
      LCD_Write(LCD_3, 1);
      break;
    case 4:
      LCD_Write(LCD_4, 1);
      break;
    case 5:
      LCD_Write(LCD_5, 1);
      break;
    case 6:
      LCD_Write(LCD_6, 1);
      break;
    case 7:
      LCD_Write(LCD_7, 1);
      break;
    case 8:
      LCD_Write(LCD_8, 1);
      break;
    case 9:
      LCD_Write(LCD_9, 1);
      break;
    default:
      LCD_Write(LCD_0, 1);
      break;
  }
}

/*
   Resets LCD mode and speed (clears screen and reprints)
*/
void resetLCD() {
  if (millis() - lcdResetTimer >= 5000) {
    LCD_Write(0x01, 0); //Clear Display
    printLCDMode();  //Print MODE:
    printLCDSpeed();  //Print SPEED:
    //updateLCDMode(mode);
    //updateLCDSpeed(speed);
    lcdResetTimer = millis();
  }
}

/**
   Initialize the LCD with proper procedure and also relevant labels of "Mode: 0" and "Speed: 0"
*/
void initializeLCD() {
  delay(15);
  LCD_Write(0x03, 0); //Function set (8 bits)
  delay(5);
  LCD_Write(0x03, 0); //Function Set (8 bits)
  delay(1);
  LCD_Write(0x03, 0); //Function Set (8 bits) (BF can be checked after this)
  LCD_Write(0x02, 0); //Function set (4 bits) (0010 0000)
  LCD_Write(0x028, 0); //Function Set (Specify 2 display lines) (0010 1000)
  LCD_Write(0x10, 0); //Display off (0000 1000)
  LCD_Write(0x0C, 0); //Display clear (0000 0001)
  LCD_Write(0x06, 0); //Entry mode set (Increment and shift to right) (0000 0111)

  printLCDMode();  //Print MODE:
  printLCDSpeed();  //Print SPEED:
  updateLCDMode(0);
  updateLCDSpeed(0);
}

/**
   Prints MODE: on first line of LCD
*/
void printLCDMode() {
  LCD_Write(B10000000, 0); //Sets cursor to start of first line
  LCD_Write(LCD_M, 1);
  LCD_Write(LCD_O, 1);
  LCD_Write(LCD_D, 1);
  LCD_Write(LCD_E, 1);
  LCD_Write(LCD_COLON, 1);
}
/**
   Prints SPEED: on second line of LCD
*/
void printLCDSpeed() {
  LCD_Write(B11000000, 0); //Sets cursor to second line
  LCD_Write(LCD_S, 1);
  LCD_Write(LCD_P, 1);
  LCD_Write(LCD_E, 1);
  LCD_Write(LCD_E, 1);
  LCD_Write(LCD_D, 1);
  LCD_Write(LCD_COLON, 1);
}

/**
   Send some command to the LCD
   Value represents D7-D0 (in bits) and RSval represents RS pin (is 0 or 1)
*/
void LCD_Write(byte value, int RSval) {
  byte dataIn = (value >> 4) & B1111; //Set first half of data
  dataIn += (RSval << 4); //Set RS bit (E bit is implicitly low)
  //Serial.println(dataIn, BIN);
  updateShiftRegister(dataIn);
  pulseE(dataIn);
  dataIn = (value & B1111); //Send 2nd half of data
  dataIn += (RSval << 4);
  updateShiftRegister(dataIn);
  pulseE(dataIn);
}

/**
   Pulse implementation for E pin for shift register
   existingData is data that we want to pulse (D7-D4 + RS)
*/
void pulseE(byte existingData) {
  delay(0.5);
  updateShiftRegister(B100000 | existingData);
  delay(0.5);
  updateShiftRegister(existingData);
  delay(0.5);
}

/**
   Update the shift register
   DataIn is in format: RS, E, D7, D6, D5, D4 (MSB FIRST)
*/
void updateShiftRegister(byte dataIn) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, dataIn); //MSBFIRST
  digitalWrite(LATCH_PIN, HIGH);
}
