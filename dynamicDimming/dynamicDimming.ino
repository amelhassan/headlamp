// This file contains code that uses the incoming LDR sensor values 
// to map the LED to an appopriate brightness level. 
// One function is a linear mapping between LDR value and brightness. 
// The other is a logrithmic mapping. 
// Rule of thumb: The less ambient light there is, the less artifical light that is needed. Have a max LDR value where there is no light.

#define LDR_PIN A5 // pin where we connected the LDR and the resistor
#define LED_PIN 13
#define LED_STATE_CHANGE 11 // digital yellow light to indicate when button is pressed
#define LED_RED 12
#define BUTTON_PIN 10
#define OFF 0
#define AUTO 1
#define RED 2
#define DIM 3
#define FULL 4
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // sets serial port for communication
  pinMode(LDR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_STATE_CHANGE, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

}

void loop() {
  // Collect all sensor information
  // LDRValue = analogRead(LDR_PIN); // read the value from the LDR

  // checking out ldr output 
  // Serial.print("LDR Value: ");
  // Serial.println(LDRValue);
  
//  brightness = autoDimLogReverse(LDRValue);
//  Serial.print("Brightness value: ");
//  Serial.println(brightness);
//  analogWrite(LED_PIN, brightness);
//  delay(100);

  testDimmer();
  delay(100);
}


byte autoDimLinear(int LDRValue) {
  int MAX_LDR = 800;
  int MIN_LDR = 0; 
  
  int MAX_BRIGHTNESS = 100;
  int MIN_BRIGHTNESS = 10;
  if (LDRValue > MAX_LDR)
    return OFF;
  else {
    // Start by linear mapping LDR values to brightness values 
      double slope = 1.0 * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) / (MAX_LDR - MIN_LDR);
      return MIN_BRIGHTNESS + round(slope * (LDRValue - MIN_LDR));
    }
}

byte autoDimLog(int LDRValue) {
  // TODO: make drops between higher LDR values less extreme, but drops 
  // between lower LDR values more extreme. Have more variability in the brightness output.
  int MAX_LDR = 800;
  int MIN_LDR = 0; 
  
  int MAX_BRIGHTNESS = log(100);
  int MIN_BRIGHTNESS = log(10);
  if (LDRValue > MAX_LDR)
    return OFF;
  else {
    // Start by linear mapping LDR values to brightness values 
      double slope = 1.0 * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) / (MAX_LDR - MIN_LDR);
      return exp(MIN_BRIGHTNESS + round(slope*(LDRValue-MIN_LDR)));
    }
}

byte autoDimLogReverse(int LDRValue) {
  // TODO: Brightness loops around to high values after a certain low LDR threshold is reached 
  // Altough there is more precision for the brightness value, it does not stay within bounds
  int MAX_LDR = 800;
  int MIN_LDR = 0; 
  
  int MAX_BRIGHTNESS = log(100);
  int MIN_BRIGHTNESS = log(10);
  if (LDRValue > MAX_LDR)
    return OFF;
  else {
    // Start by linear mapping LDR values to brightness values 
      double slope = 1.0 * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) / (MAX_LDR - MIN_LDR);
      return round((log(LDRValue)-MIN_BRIGHTNESS) / slope) + MIN_LDR;
    }
}

void testDimmer(){
  // Mock inputs, analog reads in ints between 0 and 1023 
  int mockInput[] = {120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10};
  int len = sizeof(mockInput);
  int result[len];
  
  for (int i = 0; i < len; i++){
    result[i] = autoDimLogReverse(mockInput[i]);
    Serial.print("For LDR reading of: ");
    Serial.print(mockInput[i]);
    Serial.print(" the corresponding brightness level is: ");
    Serial.print(result[i]);
    Serial.println();
   }

   // Make sure brightness is decreasing at an increasing rate
   int passed = true;   
   for (int i = 0; i < len - 2; i++){
     if (result[i] - result[i + 1] >= result[i + 1] - result[i + 2]){
        // Serial.print("Brightness value is not decreasing at an increasing rate ");
        // Serial.println();
        passed = false;
      }
    }
    if(true)
      Serial.println("TEST PASSED");
     else
       Serial.println("TEST FAILED: Brightness value is not decreasing at an increasing rate");
    delay(10000000); // slow down the main loop 
}

void debug(){
  Serial.print("LDR Value: ");
  Serial.println(LDRValue);
  Serial.print("Brightness: ");
  Serial.println(brightness);
}
