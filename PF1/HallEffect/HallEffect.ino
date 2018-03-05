//Code for Hall effect odometer

unsigned long prevInterruptLeft;
unsigned long prevInterruptRight;

const double distToCenter = 3;
int leftRPMCount = 0;

int leftRPM = 0;

const int leftInteruptPin = 2;
const int rightInteruptPin = 3;

const int M1Pin = 4;
const int E1Pin = 5;
const int E2Pin = 6;
const int M2Pin = 7;

double timeChange;

float lastmillis = millis();

void setup() {
  Serial.begin(9600);
  pinMode(leftInteruptPin, INPUT_PULLUP);
  pinMode(rightInteruptPin, INPUT_PULLUP);  
 // digitalWrite (leftInteruptPin, HIGH);
//digitalWrite (rightInteruptPin, HIGH);
//Triggers the interrupt functions when low to high detected
  attachInterrupt(digitalPinToInterrupt(leftInteruptPin), updateLeftHE, RISING);
  attachInterrupt(digitalPinToInterrupt(rightInteruptPin), updateRightHE, RISING);

  //analogWrite(E1Pin, 255);
  //janalogWrite(E2Pin, 100);
  //digitalWrite(M1Pin, LOW);
  //digitalWrite(M2Pin, LOW);
  
}

void loop() {
  

  
  
  
}

//Changes the left HE previous interrupt time after interrupt is triggered
void updateLeftHE() {
  
  timeChange = millis() - lastmillis;
  lastmillis = millis();
  String tireSpeed = String(calcTireSpeed(timeChange));
  Serial.println(timeChange);

}

void updateRightHE() {
   timeChange = millis() - lastmillis;
  lastmillis = millis();
  String tireSpeed = String(calcTireSpeed(timeChange));
  Serial.println(timeChange);
}

double calcTireSpeed(double time) {
  return (distToCenter * PI) / (time/1000); 
}


//Changes the left HE previous interrupt time after interrupt is triggered
/*void checkRightHE {
  rightSpeed = calcTireSpeed(prevInterruptRight);
  prevInterruptRight = millis();
}

//Given the interrupt time calculate the 
double calcTireSpeed(long interruptTime) {
  int tireSpeed = 0.25 * PI * ((wheelDiameter * 1000)/(millis() - interruptTime));
}

void updateLeftSpeed() {
  digitalWrite(M2Pin, LOW);
}

void updateRightSpeed() {
  digitalWrite(M1Pin, LOW);
}
*/
