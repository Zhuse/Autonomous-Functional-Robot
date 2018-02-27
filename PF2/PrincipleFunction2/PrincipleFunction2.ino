
/*
proposed sensor configuration
front of vehnical is pointing up 
* = vehical 
s = sensor 
 
         s s
         
         s s
         
         ***
        *****
        *****
         ***

two cross sensors placed directly infront of two direciton sensors

 */
int directionSensor0Pin = 0;
int directionSensor1Pin = 1;
int crossSensor0Pin = 2;
int crossSensor1Pin = 3;

int crossSensors[] = {0,0};
int directionSensors[] = {0,0};

int THRESHOLD = 500; // TODO update this value 

// direction[0] is left sensor, direction[1] is right sensor;


void setup() {
  // put your setup code here, to run once:

}


void loop() {
  principleFunction2();
}




void principleFunction2() {
  readSensors();
  updateDrive();
}



void updateDrive() {
  if (crossSensors[0] && crossSensors[1] && ~directionSensors[0] && ~directionSensors[1]) {
    // we are crossing a track, disable sensors for a bit 
    crossing();
    return; 
  } 

  if (directionSensors[0]) {
    // reduce speed of left motor
  } else if (directionSensors[1]) {
    // reduce speed of right motor
  } else {
    // full speed 
  }
}

// reduce speed of left motor
void reduceLeft() {
  
} 

// reduce speed of right motor
void reduceRight() {
  
}

// set max speed of motors 
void fullSpeed() {
  
}

// disable sensors for bit to wait for crossing
void crossing() {
  delay(200);
  return; 
}





// reads the 4 sensors and updates sensors array 
void readSensors() { 
  
  if (analogRead(directionSensor0Pin) > THRESHOLD) {
    directionSensors[0] = 1;
  } else {
    directionSensors[0] = 0;
  }

  if (analogRead(directionSensor1Pin) > THRESHOLD) {
    directionSensors[1] = 1;
  } else {
    directionSensors[1] = 0;
  }

  if (analogRead(crossSensor0Pin) > THRESHOLD) {
    crossSensors[0] = 1;
  } else {
    crossSensors[0] = 0;
  }

  if (analogRead(crossSensor1Pin) > THRESHOLD) {
    crossSensors[1] = 1;
  } else {
    crossSensors[1] = 0;
  }

  return; 
}

