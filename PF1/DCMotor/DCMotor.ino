const int MOTOR_POWER_PIN1 = 5; //E1
const int MOTOR_POLARITY_PIN1 = 4; //M1

const int MOTOR_POWER_PIN2 = 6; //E1
const int MOTOR_POLARITY_PIN2 = 7; //M1

void setup() {
  pinMode(MOTOR_POLARITY_PIN1, OUTPUT); //Direction
  pinMode(MOTOR_POLARITY_PIN2, OUTPUT); //Direction
  
  // Set initial rotation speed + direction
  analogWrite(MOTOR_POWER_PIN1, 0);
  digitalWrite(MOTOR_POLARITY_PIN1, HIGH);

  analogWrite(MOTOR_POWER_PIN2, 0);
  digitalWrite(MOTOR_POLARITY_PIN2, LOW);
}

void loop() {
  for (int i=0; i<255; i++){
    analogWrite(MOTOR_POWER_PIN1, i);
    analogWrite(MOTOR_POWER_PIN2, 255-i);
    delay(20);
  }
}
