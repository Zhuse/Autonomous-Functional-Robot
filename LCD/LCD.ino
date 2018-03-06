int dataPin = 9;
int latchPin = 10;
int clockPin = 11;

/*Characters encodings for LCD*/
const byte LCD_0 = B00110000;
const byte LCD_1 = B00110001;
const byte LCD_2 = B00110010;
const byte LCD_3 = B00110011;
const byte LCD_4 = B00110100;
const byte LCD_5 = B00110101;
const byte LCD_6 = B00110110;
const byte LCD_7 = B00110111;
const byte LCD_8 = B00111000;
const byte LCD_9 = B00111001;
const byte LCD_A = B01000001;
const byte LCD_B = B01000010;
const byte LCD_C = B01000011;
const byte LCD_D = B01000100;
const byte LCD_E = B01000101;
const byte LCD_F = B01000110;
const byte LCD_G = B01000111;
const byte LCD_H = B01001000;
const byte LCD_I = B01001001;
const byte LCD_J = B01001010;
const byte LCD_K = B01001011;
const byte LCD_L = B01001100;
const byte LCD_M = B01001101;
const byte LCD_N = B01001110;
const byte LCD_O = B01001111;
const byte LCD_P = B01010000;
const byte LCD_Q = B01010001;
const byte LCD_R = B01010010;
const byte LCD_S = B01010011;
const byte LCD_T = B01010100;
const byte LCD_U = B01010101;
const byte LCD_V = B01010110;
const byte LCD_W = B01010111;
const byte LCD_X = B01011000;
const byte LCD_Y = B01011001;
const byte LCD_Z = B01011010;
const byte LCD_SPACE = B00100000;
const byte LCD_DASH = B00101101;
const byte LCD_EXCLMARK = B00100001;
const byte LCD_COLON = B00111010;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  initialize_LCD();
}

void loop() {
  displayYouWon();
}

/**
   Displays "YOU WON!" on LCD
*/
void displayYouWon() {
  LCD_Write(0x01, 0); //Clear display
  delay(500);
  LCD_Write(B10000000, 0); //Sets cursor to home
  LCD_Write(LCD_Y, 1);
  LCD_Write(LCD_O, 1);
  LCD_Write(LCD_U, 1);
  LCD_Write(LCD_SPACE, 1);
  LCD_Write(LCD_W, 1);
  LCD_Write(LCD_O, 1);
  LCD_Write(LCD_N, 1);
  LCD_Write(LCD_EXCLMARK, 1);
}

void initialize_LCD() {
  delay(15);
  LCD_Write(0x03, 0); //Function set (8 bits)
  delay(5);
  LCD_Write(0x03, 0); //Function Set (8 bits)
  delay(1);
  LCD_Write(0x03, 0); //Function Set (8 bits) (BF can be checked after this)
  LCD_Write(0x02, 0); //Function set (4 bits) (0010 0000)
  LCD_Write(0x028, 0); //Function Set (Specify 2 display lines) (0010 1000)
  LCD_Write(0x10, 0); //Display off (0000 1000)
  LCD_Write(0x0C, 0); //Display clear (0000 0001)
  LCD_Write(0x06, 0); //Entry mode set (Increment and shift to right) (0000 0111)
}

void LCD_Write(byte value, int RSval) {
  byte dataIn = (value >> 4); //Set first half of data
  dataIn += (RSval << 5); //Set RS bit
  //Serial.println(dataIn, BIN);
  updateShiftRegister(dataIn);
  pulseE(dataIn);
  dataIn = (value & B1111); //Send 2nd half of data
  dataIn += (RSval << 5);
  updateShiftRegister(dataIn);
  pulseE(dataIn);
}

void pulseE(byte existingData) {
  delay(0.5);
  updateShiftRegister(B010000 | existingData);
  delay(0.5);
  updateShiftRegister(B000000 | existingData);
  delay(0.5);
}

//DataIn is in format: RS, E, D7, D6, D5, D4 (MSB FIRST)
void updateShiftRegister(byte dataIn) {
  digitalWrite(latchPin, LOW);
  Serial.println(dataIn, BIN);

  shiftOut(dataPin, clockPin, LSBFIRST, dataIn); //MSBFIRST
  digitalWrite(latchPin, HIGH);
}

void printDigits(int number){
    if(number < 10){
        //print "number" to LCD
    }
    else{
        printDigits(number / 10);
        //print "number % 10" to LCD
    }
}
