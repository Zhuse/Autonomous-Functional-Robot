#define TCRT5000_SENSOR_PIN A2

void setup() {
  Serial.begin(9600);
}

void loop() {
  int reading = analogRead(TCRT5000_SENSOR_PIN);  
  // read is 0..1024, write is 0..255
  Serial.print("test ");
  Serial.println(reading);
    
}
