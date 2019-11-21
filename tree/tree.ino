#define LDRpin A0 // pin where we connected the LDR and the resistor
#define LEDpin 9
#define off 0
#define dim 50
#define full 255
#define LDR_thresh 1000

// Defintion of states (5 steps)
  // 0:off, 1:dim_manually, 2:full_manually, 3:dim_dynamically, 4: full_dynamically, should we have a ramping state?  
// TODO: dims 

// Button Vars
int buttonPin = 2;
int buttonState = 0;
bool pressed = false;

int state = 0;
bool ramping = false;
int brightness = 0;
int LDRValue;

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
  if (buttonRead()){
      cycle();
      debug();
  } else if (LDRValue > LDR_thresh ){
    if (state == 2)
      dimState();
      debug();
  } else if (LDRValue <= LDR_thresh ){
    if(state == 3)
      cycle();
      debug();
  }
  setBrightness();
  analogWrite(LEDpin, brightness);
  delay(100);
}

void debug(){
  String to_print = "State: " + state;
  Serial.println(state);
  Serial.println(LDRValue);
  //Serial.println(to_print); 
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

void setBrightness(){
  if (state == 0)
    brightness = off;
  else if (state == 1 or state == 3)
    brightness = dim;
  else 
    brightness = full;
  
}

void getState() {
  if (brightness = off)
    state = 0;
   else if (brightness > off and brightness < full)
    state = 1;
   else if (state = full)
    state = 2;
  }


void cycle() {
  if (state == 0) {
    state = 1;
  } else if (state == 1) {
    if (ramping) 
      state = 0;
    else 
      state = 2;
  } else if (state == 2) {
    state = 0;
  } else if (state == 3){
    state = 2;
  }
}

void dimState() {
  state = 3;
} 
