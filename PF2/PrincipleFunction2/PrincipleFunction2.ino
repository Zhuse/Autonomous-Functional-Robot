
/*
proposed sensor configuration
front of vehnical is pointing up 
* = vehical 
s = sensor 
 
       s s s s
         
         ***
        *****
        *****
         ***

two cross sensors placed directly infront of two direciton sensors

*/

const int maxSpeed = 255;

 
const int opticalSensor0Pin = A0;
const int opticalSensor1Pin = A1;
const int opticalSensor2Pin = A2;
const int opticalSensor3Pin = A3;

const int opticalLED0Pin = 8;
const int opticalLED1Pin = 9;
const int opticalLED2Pin = 10;
const int opticalLED3Pin = 11;




const int leftMotorPower = 5; //E1 (left wheel speed)
const int leftMotorPolarity = 4; //M1 (left wheel) , LOW is forward
const int rightMotorPower = 6; //E1 (right wheel speed)
const int rightMotorPolarity = 7; //M1 (right wheel), HIGH is forward

const int leftFwd = LOW;
const int leftBwd = HIGH;

const int rightFwd = HIGH;
const int rightBwd = LOW;





int opticalSensors[] = {0,0,0,0};

int THRESHOLD = 500; // TODO update this value 

// optical[0] is furthest left sensor
// optical[1] is furthest right sensor


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(opticalLED0Pin, OUTPUT);
  pinMode(opticalLED1Pin, OUTPUT);
  pinMode(opticalLED2Pin, OUTPUT);
  pinMode(opticalLED3Pin, OUTPUT);

  pinMode(opticalSensor0Pin, INPUT);
  pinMode(opticalSensor1Pin, INPUT);
  pinMode(opticalSensor2Pin, INPUT);
  pinMode(opticalSensor3Pin, INPUT);


  pinMode(leftMotorPolarity, OUTPUT); //Direction
  pinMode(rightMotorPolarity, OUTPUT); //Direction
  
  // Set initial rotation speed to 0
  analogWrite(leftMotorPower, 0);
  analogWrite(rightMotorPower, 0);

  digitalWrite(leftMotorPolarity, leftFwd);
  digitalWrite(rightMotorPolarity, rightFwd);

}


void loop() {
  principleFunction2();
}




void principleFunction2() {
  readSensors();
  updateDrive();
}



void updateDrive() {
  if (~opticalSensors[0] && ~opticalSensors[1]) {
    // we are crossing a track, disable sensors for a bit 
    crossing();
    return; 
  } 

  if (opticalSensors[0]) {
 		reduceleft(2);	    // reduce speed of left motor a lot
  } else if (opticalSensors[1]) {
 		reduceleft(1);	    // reduce speed of left motor a bit
  } else if (opticalSensors[2]) {
  	reduceright(1);    // reduce speed of right motor a bit
	} else if (opticalSensors[3]) {
  	reduceright(2);    // reduce speed of right motor a lot 
	} else {
    fullSpeed();		// full speed
  }
}

// reduce speed of left motor depending on value of spd
// if spd == 1 reduce a bit, if spd == 2 reduce a lot
void reduceleft(int spd) {
  if (spd == 1) {
    analogWrite(leftMotorPower, maxSpeed - 20);
	} else if (spd == 2) {
    analogWrite(leftMotorPower, maxSpeed - 50);
	}  
} 

// reduce speed of right motor
// if spd == 1 reduce a bit, if spd == 2 reduce a lot
void reduceright(int spd) {
  if (spd == 1) {
    analogWrite(rightMotorPower, maxSpeed - 20);
  } else if (spd == 2) {
    analogWrite(rightMotorPower, maxSpeed - 50);
	}  
}

// set max speed of motors 
void fullSpeed() {
  analogWrite(leftMotorPower, maxSpeed);
  analogWrite(rightMotorPower, maxSpeed);
}

// disable sensors for bit to wait for crossing
void crossing() {
  delay(200);
  return; 
}










// reads the 4 sensors and updates sensors array 
void readSensors() { 
  
  if (analogRead(opticalSensor0Pin) > THRESHOLD) {
    opticalSensors[0] = 1;
  } else {
    opticalSensors[0] = 0;
  }

  if (analogRead(opticalSensor1Pin) > THRESHOLD) {
    opticalSensors[1] = 1;
  } else {
    opticalSensors[1] = 0;
  }

  if (analogRead(opticalSensor2Pin) > THRESHOLD) {
    opticalSensors[2] = 1;
  } else {
    opticalSensors[2] = 0;
  }

  if (analogRead(opticalSensor3Pin) > THRESHOLD) {
    opticalSensors[3] = 1;
  } else {
    opticalSensors[3] = 0;
  }

  return; 
}
