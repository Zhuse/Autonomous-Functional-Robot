int dataPin = 9;
int latchPin = 10;
int clockPin = 11;

byte dataIn = 0;

void setup() {
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
}

void loop() {
   dataIn = 0;
   
}

void updateShiftRegister() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, dataIn); //MSBFIRST
  digitalWrite(latchPin, HIGH);
}

