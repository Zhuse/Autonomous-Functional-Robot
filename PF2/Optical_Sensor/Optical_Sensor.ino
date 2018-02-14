const int opticalSensorLED = 1; //digital
const int opticalSensorPhotocell = 1; //analog
int opticalSensorReading; //high reading means more light detected. This is the difference between noise + LED signal and noise to end up with LED signal
int noiseAndSignal;
int noise;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(opticalSensorLED, OUTPUT);
  pinMode(opticalSensorPhotocell, INPUT);
  

}

void loop() {
  //turn on LED here and read the signal from photocell to get noise + signal
  digitalWrite(opticalSensorLED, HIGH);
  delay(50);
  noiseAndSignal = analogRead(opticalSensorPhotocell);

  //turn off LED and read from photocell
  digitalWrite(opticalSensorLED, LOW);
  delay(50);
  noise = analogRead(opticalSensorPhotocell);

  //compute difference
  opticalSensorReading = noiseAndSignal - noise;
  Serial.println(opticalSensorReading);
}
