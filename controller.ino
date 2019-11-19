Authored by:
    - Matt Daly <mdaly04@tufts.edu>

// LED Vars
int ledPin = 9;
byte brightness = 255;
int mode = 0; // 0 = off, 1 = dimmed, 2 = 

// Dimming
bool dimmed = false;

// Ramping
byte ramp = 105

// Sensor Vars
#define LDRpin A0 // pin where we connected the LDR and the resistor
int LDRValue = 0;     // result of reading the analog pin

// Button Vars
int buttonPin = 2;

bool pressed = false;

void setup() {
  Serial.begin(9600); // sets serial port for communication
  pinMode(ledPin, OUTPUT);

}

void loop() {
  // Collect all sensor information
  LDRValue = analogRead(LDRpin); // read the value from the LDR
  buttonState = didgitalRead(buttonPin);
  Serial.print("Button value: ");
  Serial.print(buttonState);
  Serial.print("\n");
  if (buttonState == HIGH and not pressed) {
    brightness = cycle(brightness);
    pressed = true;
  } 
  else if (buttonState == LOW and pressed) {
    pressed = false;
  }
  
  // Display debug info
  info();

  delay(100); 
  // Decision tree
  // Higher LDRValue == Brighter Environment
  dimming();
}

byte cycle(byte b) {
  if (b == 0) {
    b = 10;
  } else if (b == 10) {
    b = 255;
  } else if (b == 255) {
    b = 0;
  }
  return b;
}

void dimming() {
  if (LDRValue > 1000){
      Serial.println("dimming");
      brightness = 105;
      analogWrite(ledPin, brightness); // dims 
      dimmed = true;
  } else if (dimmed) {
    ramp = rampUp(ramp);
  } else {
    brightness = 255;
    analogWrite(ledPin, brightness);
  }

}

byte rampUp(byte r){
  Serial.println("Ramping");
  info();
  if (r == 255) {
    dimmed = false;
    return 105;
  } else {
    r += 50;
    analogWrite(ledPin, r)
    delay(1000)
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
