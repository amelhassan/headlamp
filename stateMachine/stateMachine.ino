// PINS
#define LDR_PIN A5 // A0, pin where we connected the LDR and the resistor
#define LED_PIN 11 // 11
#define BUTTON_PIN 6 // 5
#define LED_STATE_CHANGE_PIN 9 // 9 digital yellow light to indicate when button is pressed
#define LED_RED_PIN 10 // 10

// STATES
#define OFF 0
#define AUTO 1
#define RED 2
#define DIM 3
#define FULL 4
// VALUES
#define LDR_thresh 1000
#define DIM_VAL 20
#define FULL_VAL 255

// Defintion of states (4 states)
  // off, auto, red, dim, full, off

// Button Vars
int buttonState = 0;
bool pressed = false;

int state = OFF;
int brightness = OFF;

int LDRValue;
int delta;

bool ramping = false;

// Control rate of auto state 
uint32_t start_auto; 
uint32_t end_auto;
uint32_t autoRate = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // sets serial port for communication
  pinMode(LDR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_STATE_CHANGE_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  start_auto = millis();
  end_auto = millis();
}

void loop() {
  // uint32_t ts1 = millis(); // Timing code
   
  // Collect all sensor information
  LDRValue = analogRead(LDR_PIN); // read the value from the LDR
  buttonState = digitalRead(BUTTON_PIN);
  // debug();

  // Changes state if necessary
  if (buttonRead()){
      cycle();
      delay(100);
  } 
  analogWrite(LED_STATE_CHANGE_PIN, OFF);
  end_auto = millis();
  // Actions based on state 
  if (state == OFF)
    ramp(OFF);
  else if (state == AUTO){
    if (end_auto - start_auto > autoRate)
      ramp(autoLogReverse(LDRValue)); // end and start used to control rate of change
  }
  else if (state == RED){
    ramp(OFF);
    analogWrite(LED_RED_PIN, FULL_VAL);
  }
  else if (state == DIM){
    analogWrite(LED_RED_PIN, OFF);
    ramp(DIM_VAL);
  }
  else if (state == FULL)
    ramp(FULL_VAL);
  else{
    Serial.print("State number not valid: ");
    Serial.println(state);
    }

  analogWrite(LED_PIN, brightness);
  delay(100);

// Timing code 
//  uint32_t ts2 = millis();
//  Serial.print("Loop time elapsed in ms: ");
//  Serial.println(ts2-ts1);
}

bool buttonRead(){
  if (buttonState == HIGH and not pressed) {
    // Serial.println("BUTTON PRESSED\n");
    pressed = true;
    return true;
  } 
  else if (buttonState == LOW and pressed) {
    pressed = false;
  }
  return false;
}

void cycle() {
  analogWrite(LED_STATE_CHANGE_PIN, FULL_VAL);
  if (state == FULL)
    state = OFF;
  else{
    state++;
  }
}

void ramp(int target){
  if(brightness != target) 
    ramping = true;
  if (ramping){
      if(target > brightness)
        rampUp(target);
      else 
        rampDown(target);
    }
  }

 void rampUp(int target){
  int original_brightness = brightness;
  float delta = 0.02;
  // Calculate current percentage of perceived brightness based on current val of brightness 
  float perceived_percent = sqrt((float)brightness / FULL_VAL); // gives a percentage 

  // Increase the percent perceived brightness by delta 
  float perceived_goal_percent = perceived_percent + delta;
  
  // Calculate the measured brightness needed to get the perceived goal percent
  float measured_brightness = sq(perceived_goal_percent);
  if(measured_brightness >= ((float)target/FULL_VAL)){
      brightness = target;
      ramping = false; // bool for ramping to stop 
      
   }else{
      brightness = FULL_VAL * measured_brightness; // Note: First product should not round down to 0, or infinite loop
      if(brightness == original_brightness){ // Otherwise, potential for infinite loop
          brightness+=5;
          if (brightness > target){
            brightness = target;
            ramping = false;
           }
        }
    }
}

void rampDown(int target){
  int original_brightness = brightness;
  float delta = 0.02;
  // Calculate current percentage of perceived brightness based on current val of brightness 
  float perceived_percent = sqrt((float)brightness / FULL_VAL); // gives a percentage 

  // Decrease the percent perceived brightness by delta 
  float perceived_goal_percent = perceived_percent - delta;
  
  if (perceived_goal_percent < 0){
    brightness = target;
    ramping = false;
    return;
    }
  
  // Calculate the measured brightness needed to get the perceived goal percent
  float measured_brightness = sq(perceived_goal_percent); // Problem: negative perceived may become positive 
  if(measured_brightness <= ((float)target/FULL_VAL)){
      brightness = target;
      ramping = false; // bool for ramping to stop 
      
   }else{
      // Serial.print("Measured brightness percent: ");
      // Serial.println(measured_brightness);
      brightness = FULL_VAL * measured_brightness; 
      if(brightness == original_brightness){ // Otherwise, potential for infinite loop
          brightness-=5;
          if (brightness < target){
            brightness = target;
            ramping = false;
            } 
        }
    }
}

// Function for Auto state
// "Exponential" mapping between LDR value (ambient light) and output brightness
byte autoLogReverse(int LDRValue) {
  start_auto = millis();
  float MIN_LDR = 10; 
  float MAX_LDR = 1000;

  float MIN_BRIGHTNESS = 10;
  float MAX_BRIGHTNESS = 200;
  
  if (LDRValue > MAX_LDR)
    return OFF;
  else if (LDRValue < MIN_LDR){
    return MIN_BRIGHTNESS;
    }
  else {
     // LDR to sinh mapping 
     double sinh_val = sinhMap(LDRValue);
     // sinh to linear brightness mapping
     double MIN_IN = sinhMap(MIN_LDR);
     double MAX_IN = sinhMap(MAX_LDR);
     double slope = 1.0 * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) / (MAX_IN - MIN_IN);
     return (byte) MIN_BRIGHTNESS + round(slope * (sinh_val - MIN_IN));
    }
}

// sinh function for autoLogReverse function
double sinhMap(double y){
    return log10((y + sqrt(pow(y, 2) + 4))/2);
  }

void debug(){
//  Serial.print("State: ");
//  Serial.println(state);
  Serial.print("LDR Value: ");
  Serial.println(LDRValue);
  Serial.print("Brightness: ");
  Serial.println(brightness);
  
}
