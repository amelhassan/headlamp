#define LDRpin A0 // pin where we connected the LDR and the resistor
int ledPin = 3;
int LDRValue = 0;     // result of reading the analog pin
byte brightness = 255;
byte ramp = 105;
bool dimmed = false;

void setup() {
  Serial.begin(9600); // sets serial port for communication
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Collect all sensor information
  LDRValue = analogRead(LDRpin); // read the value from the LDR
  Serial.println(LDRValue);
//  // Display debug info
//  info();
//
//  delay(100); 
//  // Decision tree
//  // Higher LDRValue == Brighter Environment
//  if (LDRValue > 1000){
//      Serial.println("dimming");
//      brightness = 105;
//      analogWrite(ledPin, brightness); // dims 
//    dimmed = true;
//  } else if (dimmed) {
//    ramp = rampUp(ramp);
//  } else {
//    brightness = 255;
//    analogWrite(ledPin, brightness);
//  }
}

byte rampUp(byte r){
  Serial.println("Ramping");
  info();
  if (r == 255) {
    dimmed = false;
    return;
  } else {
    r += 50;
    analogWrite(ledPin, r);
    delay(1000);
    return r;
  }
}

void info(){
  Serial.print("LDR: ");
  Serial.print(LDRValue);
  Serial.print('\n');
  Serial.print("Brightness: ");
  Serial.print(brightness);
  Serial.print('\n');
}
