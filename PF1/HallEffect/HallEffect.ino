//Code for Hall effect odometer

unsigned long prevInterruptLeft;
unsigned long prevInterruptRight;

const double wheelDiamter = 6.0;

const int leftInteruptPin = 2;
const int rightInteruptPin = 3;

const int M1Pin = 4;
const int E1Pin = 5;
const int E2Pin = 6;
const int M2Pin = 7;

void setup() {
  Serial.begin(9600);
  pinMode(leftInteruptPin, INPUT_PULLUP);
  pinMode(rightInteruptPin, INPUT_PULLUP);  

//Triggers the interrupt functions when low to high detected
  attachInterrupt(digitalPinToInterrupt(leftInteruptPin), checkLeftHE, RISING);
//  attachInterrupt(digitalPinToInterrupt(rightInteruptPin), checkRightHE, RISING);

  analogWrite(E1Pin, 255);
  analogWrite(E2Pin, 100);
  digitalWrite(M1Pin, LOW);
  digitalWrite(M2Pin, LOW);
  
}

void loop() {
}

//Changes the left HE previous interrupt time after interrupt is triggered
void checkLeftHE() {
  Serial.println("detect");
  //leftSpeed = calcTireSpeed(prevInterruptLeft);
  prevInterruptLeft = millis();
  Serial.println(prevInterruptLeft);
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
