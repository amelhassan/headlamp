#define LDRpin A0 // pin where we connected the LDR and the resistor
#define LEDpin 9
#define OFF 0
#define AUTO 1
#define DIM 2
#define FULL 3
#define LDR_thresh 1000
#define DIM_VAL 20
#define FULL_VAL 255

// Defintion of states (4 states)
  // off, auto, dim,full,off

// Button Vars
int buttonPin = 2;
int buttonState = 0;
bool pressed = false;

int state = OFF;
int brightness = 0;
int LDRValue;
int delta;

bool ramping = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // sets serial port for communication
  pinMode(LEDpin, OUTPUT);

}

void loop() {
  // Collect all sensor information
  LDRValue = analogRead(LDRpin); // read the value from the LDR
  buttonState = digitalRead(buttonPin);
  debug();

  // Changes state if necessary
  if (buttonRead()){
      cycle();
      debug();
  } 

  // Actions based on state 
  if (state == OFF)
    brightness = ramp(OFF);
  else if (state == AUTO)
    brightness = autoDim(LDRValue);
  else if (state == DIM) 
    brightness = ramp(DIM_VAL);
  else if (state == FULL)
    brightness = ramp(FULL_VAL);
  else 
    Serial.print("State number not valid\n");
    
  analogWrite(LEDpin, brightness);
  delay(100);
}

byte ramp(byte target) {
  if (ramping) {
    delta = delta * 2;
  } else {
    delta = (target - brightness) / 8;
    ramping = true;
  }
  if (brightness + 40 > target or brightness - 40 < target) {
    return target;
  }
  return brightness + delta;
  // int delta = (target - brightness) / 2;
//  if(abs(delta) < 10 ) // threshold for error
//    return target;
//  return brightness + delta;
}

byte autoDim(int LDRValue) {
  // Eventually log mapping
  if (LDRValue > 1000)
    return DIM_VAL;
  else 
    return FULL_VAL;
}

void debug(){
  Serial.print("State: ");
  Serial.println(state);
  Serial.print("LDR Value: ");
  Serial.println(LDRValue);
  Serial.print("Brightness: ");
  Serial.println(brightness);
  
}

bool buttonRead(){
  if (buttonState == HIGH and not pressed) {
    Serial.print("BUTTON PRESSED\n");
    pressed = true;
    return true;
  } 
  else if (buttonState == LOW and pressed) {
    pressed = false;
  }
  return false;
}

void cycle() {
  if (state == FULL)
    state = OFF;
  else 
    state++;
}
