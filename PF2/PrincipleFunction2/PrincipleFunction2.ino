
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
int directionSensor0Pin = 0;
int directionSensor1Pin = 1;
int directionSensor2Pin = 2;
int directionSensor3Pin = 3;

int directionSensors[] = {0,0,0,0};

int THRESHOLD = 500; // TODO update this value 

// direction[0] is furthest left sensor
// direction[1] is furthest right sensor


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
  if (~directionSensors[0] && ~directionSensors[1]) {
    // we are crossing a track, disable sensors for a bit 
    crossing();
    return; 
  } 

  if (directionSensors[0]) {
    // reduce speed of left motor a lot
 		reduceLeft(2);	
  } else if (directionSensors[1]) {
    // reduce speed of left motor a bit
 		reduceLeft(1);	
  } else if (directionSensors[2]) {
    // reduce speed of right motor a bit
  	reduceRight(1);
	} else if (directionSensors[3]) {
    // reduce speed of right motor a lot 
  	reduceRight(2);
	} else {
		// full speed
  }
}

// reduce speed of left motor depending on value of spd
// if spd == 1 reduce a bit, if spd == 2 reduce a lot
void reduceLeft(int spd) {
  if (spd == 1) {
		// reduce speed a bit
	} else if (spd == 2) {
		// reduce speed a lot
	}  
} 

// reduce speed of right motor
// if spd == 1 reduce a bit, if spd == 2 reduce a lot
void reduceRight(int spd) {
  if (spd == 1) {
		// reduce speed a bit
	} else if (spd == 2) {
		// reduce speed a lot
	}  
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

  if (analogRead(directionSensor2Pin) > THRESHOLD) {
    directionSensors[2] = 1;
  } else {
    directionSensors[2] = 0;
  }

  if (analogRead(directionSensor3Pin) > THRESHOLD) {
    directionSensors[3] = 1;
  } else {
    directionSensors[3] = 0;
  }

  return; 
}
