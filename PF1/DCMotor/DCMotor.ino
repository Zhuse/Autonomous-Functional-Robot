const int MOTOR_POWER_PIN = 1; //E1
const int MOTOR_POLARITY_PIN = 2; //M1

void setup() {
  pinMode(MOTOR_POWER_PIN, OUTPUT); //PWM->controls speed (0-255)
  pinMode(MOTOR_POLARITY_PIN, OUTPUT); //Direction
  
  // Set initial rotation speed + direction
  analogWrite(MOTOR_POWER_PIN, 0);
  digitalWrite(MOTOR_POLARITY_PIN, HIGH);
}

void loop() {
  for (int i=0; i<10; i++){
    analogWrite(MOTOR_POWER_PIN, i*10+10);
  }
}
