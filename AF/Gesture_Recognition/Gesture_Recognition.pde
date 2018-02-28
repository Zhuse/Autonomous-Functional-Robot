import de.voidplus.leapmotion.*;
LeapMotion leap;

/*
proposed sensor configuration
front of vehnical is pointing up 
* = car 
s = direction sensor
c = cross sensor
 
       s1 c1 c2 s2
         
         ***
        *****
        *****
         ***

two cross sensors placed in middle
*/

//TODO update port values
final int MOTOR_PORT = 1;
final int S1_LED_PORT = 2;
final int S1_PC_PORT = 2; //ANALOG
final int C1_LED_PORT = 3;
final int C1_PC_PORT = 3; //ANALOG
final int C2_LED_PORT = 4;
final int C2_PC_PORT = 4; //ANALOG
final int S2_LED_PORT = 5;
final int S2_PC_PORT = 5; //ANALOG

final int LIGHT_THRESHOLD = 500; 

void setup(){
  size(800, 500);
  background(255);
  leap = new LeapMotion(this).allowGestures();  //Let leap motion detect all gestures
}

void draw(){
  
}

void leapOnSwipeGesture(SwipeGesture g, int state){
  int     id               = g.getId();
  Finger  finger           = g.getFinger();
  PVector position         = g.getPosition();
  PVector positionStart    = g.getStartPosition();
  PVector direction        = g.getDirection();
  float   speed            = g.getSpeed();
  long    duration         = g.getDuration();
  float   durationSeconds  = g.getDurationInSeconds();
  
  
}

void leapOnCircleGesture(CircleGesture g, int state){
  int direction = g.getDirection();
  //assuming direction is 0 for clockwise, 1 for counter-clockwise
  if(direction == 0){
     //do a clockwise spin
     //TODO choice of middle-left sensor kind of arbitrary
     //while the middle-left sensor still detects black tape, spin clockwise to get the sensor off the tape
     while(tapeDetected(C1_LED_PORT, C1_PC_PORT)){
       
     }
     //sensor is now off the tape. while the middle-left sensor has not detected black tape, spin clockwise to get the sensor on the tape
  }
  else{
     //do a counter-clockwise spin 
  }
}

boolean tapeDetected(final int sensorLEDPort, final int sensorPhotocellPort){
    //read from sensor port
    if(readSensor(sensorLEDPort, sensorPhotocellPort) > LIGHT_THRESHOLD){
        //then no tape detected
        return false;
    }
    return true;
}

int readSensor(final int sensorLEDPort, final int sensorPhotocellPort){
  int noiseAndSignal;
  int noise;
  int opticalSensorReading;
  
  //turn on LED here and read the signal from photocell to get noise + signal
  digitalWrite(sensorLEDPort, HIGH);
  delay(50);
  noiseAndSignal = analogRead(sensorPhotocellPort);

  //turn off LED and read from photocell
  digitalWrite(sensorLEDPort, LOW);
  delay(50);
  noise = analogRead(sensorPhotocellPort);

  //compute difference
  opticalSensorReading = noiseAndSignal - noise;
  return opticalSensorReading;
}