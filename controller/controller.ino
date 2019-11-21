#define LDRpin A0 // pin where we connected the LDR and the resistor
#define LEDpin 9
#define off 0
#define dim 50
#define full 255
// LED Vars
byte brightness = full;

// Dimming
bool dimmed = false;

// Ramping
byte ramp = dim;

// Sensor Vars
int LDRValue = 0;     // result of reading the analog pin

// Button Vars
int buttonPin = 2;
int buttonState = 0;
bool pressed = false;

void setup() {
  Serial.begin(9600); // sets serial port for communication
  pinMode(LEDpin, OUTPUT);

}

void loop() {
  // Collect all sensor information
  LDRValue = analogRead(LDRpin); // read the value from the LDR
  buttonState = digitalRead(buttonPin);
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
  analogWrite(LEDpin, brightness);
  
  // Display debug info
  info();

  delay(100); 
  // Decision tree
  // Higher LDRValue == Brighter Environment
  dimming();
}


byte cycle(byte b) {
  if (b <= full and b > dim ) {
    b = off;
    
  } else if (b <= dim and b > 0 ) {
    b = full;
    
  } else if (b == 0) {
    b = dim;
  }
  return b;
}

void dimming() {
  if (LDRValue > 1000){
      Serial.println("dimming");
      if (brightness != 0) {
        brightness = 105;
      }
      analogWrite(LEDpin, brightness); // dims 
      dimmed = true;
  } else if (dimmed) {
    ramp = rampUp(ramp);
    brightness = ramp; // MIGHT BE WRONG
  } // else {
   // analogWrite(ledPin, brightness);
 // }

}

byte rampUp(byte r){
  Serial.println("Ramping");
  info();
  if (r == 255) {
    dimmed = false;
    return ;
  } else {
    if ((r + 50) > 255) { r = 255; } else { r += 50; }
    analogWrite(LEDpin, r);
    delay(10000);
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
