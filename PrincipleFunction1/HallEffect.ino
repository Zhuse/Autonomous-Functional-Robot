unsigned long prevInterruptLeft;
unsigned long prevInterruptRight;

const int leftInteruptPin = 2;
const int rightInteruptPin = 3;

void setup() {
	pinMode(leftInteruptPin, INPUT_PULLUP);
//Triggers the interrupt functions when low to high detected
	attachInterrupt(digitalPinToInterrupt(leftInteruptPin), checkLeftHE, RISING);
	attachInterrupt(digitalPinToInterrupt(rightInteruptPin), checkRightHE, RISING);

}

void loop() {


}

void checkLeftHE() {
	leftSpeed = 0.50 * PI;
	prevInterruptLeft = millis();
}

void checkRightHE {
	rightSpeed = 0.50 * PI;
	prevInterruptRight = millis();
}

double calcTireSpeed() {

}

void updateLeftSpeed() {

}

void update rightSpeed() {

}
