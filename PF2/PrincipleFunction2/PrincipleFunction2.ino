
/*
proposed sensor configuration
front of vehnical is pointing up 
* = vehical 
s = sensor 
 
        
     S1 S2 S3 S4         
         ***
        *****
        *****
         ***

two cross sensors placed directly infront of two direciton sensors

 */
const int directionSensor0Pin = 0;
const int directionSensor1Pin = 1;
const int crossSensor0Pin = 2;
const int crossSensor1Pin = 3;
const int motorPin = 4;

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

//increase speed of left motor
void increaseLeft() {
  
}

// reduce speed of right motor
void reduceRight() {
  
}

//increase speed of right motor
void increaseRight(){
  
}

//make a hard right (case when S3 and S4 detect black tape only)
void hardRight(){
  
}

//make a hard left (case when S1 and S2 detect black tape only)
void hardLeft(){
  
}

//make a soft right (case when S3 detects black tape only)
void softRight(){
  
}

//make a soft left (case when S2 detects black tape only)
void softLeft(){
  
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

