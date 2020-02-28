// This file dims in such a manner that for a given time step, the measured 
// brightness decreases in a way to keep the precieved brightness constant.
#define LDRpin A0 // pin where we connected the LDR and the resistor
#define LEDpin 9
#define OFF 0
#define AUTO 1
#define DIM 2
#define FULL 3
#define LDR_thresh 1000
#define DIM_VAL 20
#define FULL_VAL 255

byte brightness = FULL_VAL;
bool ramping = true;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // sets serial port for communication
  pinMode(LEDpin, OUTPUT);

}

void loop() {

      if(ramping)
          ramp(OFF);
      analogWrite(LEDpin, brightness);
      Serial.print("BRIGHTNESS VALUES: ");
      Serial.println(brightness);
      delay(200);
}

void ramp(int target){
  if(target > brightness)
    rampUp(target);
  else 
    rampDown(target);
  }
void rampUp(int target){
  int original_brightness = brightness;
  float delta = 0.02;
  // Calculate current percentage of perceived brightness based on current val of brightness 
  float perceived_percent = sqrt((float)brightness / FULL_VAL); // gives a percentage 

      Serial.print("Percieved percent: ");
      Serial.println(perceived_percent);

  // Increase the percent perceived brightness by delta 
  float perceived_goal_percent = perceived_percent + delta;
        Serial.print("Percieved percent GOAL: ");
      Serial.println(perceived_goal_percent);
  
  // Calculate the measured brightness needed to get the perceived goal percent
  float measured_brightness = sq(perceived_goal_percent);
  if(measured_brightness >= ((float)target/FULL_VAL)){
      brightness = target;
      ramping = false; // bool for ramping to stop 
      
   }else{
      Serial.print("Measured brightness percent: ");
      Serial.println(measured_brightness);
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

      Serial.print("Percieved percent: ");
      Serial.println(perceived_percent);

  // Decrease the percent perceived brightness by delta 
  float perceived_goal_percent = perceived_percent - delta;
        Serial.print("Percieved percent GOAL: ");
      Serial.println(perceived_goal_percent);
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
      Serial.print("Measured brightness percent: ");
      Serial.println(measured_brightness);
      brightness = FULL_VAL * measured_brightness; // Note: First product should not round down to 0, or infinite loop
      if(brightness == original_brightness){ // Otherwise, potential for infinite loop
          brightness-=5;
          if (brightness < target){
            brightness = target;
            ramping = false;
            } 
        }
    }
}
