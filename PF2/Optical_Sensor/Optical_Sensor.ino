//http://www.instructables.com/id/Using-IR-Sensor-TCRT-5000-With-Arduino-and-Program/
//instead of digital in, connect it to 5V
const int 
const int SENSOR_PORT = 0; //analog
const int THRESHOLD = 350;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SENSOR_PORT, INPUT);
}

void loop() {
  delay(500);
  int value = analogRead(SENSOR_PORT);
   Serial.println(value);
//  if(value < THRESHOLD){
//      Serial.println("black tape detected");
//  }
//  else{
//    Serial.println("no black tape detected");
//  }
  
}


//void setup()
//
//{
//
//Serial.begin(9600);
//
//
//
//}
//
//void loop()
//
//{
//
//delayMicroseconds(500);
//
//a=analogRead(A3);
//
//Serial.println(a); }
