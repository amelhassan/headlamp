// This file contains code that uses the incoming LDR sensor values 
// to map the LED to an appopriate brightness level. 
// One function is a linear mapping between LDR value and brightness. 
// The other is a logrithmic mapping. 
// Rule of thumb: The less ambient light there is, the less artifical light that is needed. Have a max LDR value where there is no light.
// Minimum lumens should be around 40

#define LDR_PIN A5 
#define LED_PIN 11
#define LED_STATE_CHANGE 9 // yellow light to indicate when button is pressed
#define LED_RED 10
#define BUTTON_PIN 6
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
  LDRValue = analogRead(LDR_PIN); // read the value from the LDR
  brightness = autoDimLogReverse(LDRValue);
  Serial.print("Brightness value: ");
  Serial.println(brightness);
  Serial.print("LDR value: ");
  Serial.println(LDRValue);
  
  analogWrite(LED_PIN, brightness);
  delay(100);

  // testDimmer();
  delay(100);
}

// "Exponential" mapping between LDR value (ambient light) and output brightness
// FUNCTION TO IMPLEMENT IN STATE MACHINE
byte autoDimLogReverse(int LDRValue) {
  
  float MIN_LDR = 10; 
  float MAX_LDR = 400;

  float MIN_BRIGHTNESS = 10;
  float MAX_BRIGHTNESS = 100;
  
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

// sinh function
double sinhMap(double y){
    return log10((y + sqrt(pow(y, 2) + 4))/2);
  }

// TESTS 

// Test with mock inputs 
void testDimmer(){
  
  // Mock inputs, analog reads in ints between 0 and 1023 
  int mockInput[] = {400, 350, 300, 250, 200, 150, 100, 50, 0};
  int len = sizeof(mockInput)/sizeof(mockInput[0]);
  int result[len];
  
  for (int i = 0; i < len; i++){
    result[i] = autoDimLogReverse(mockInput[i]);
    Serial.print("For LDR reading of: ");
    Serial.print(mockInput[i]);
    Serial.print(" the corresponding brightness level is: ");
    Serial.print(result[i]);
    Serial.println();
   }

   /////// TESTS /////////
  testInRange(result, len);
  testDecInc(result, len);
  // delay(1000); // slow down the main loop 
}

// Make sure brightness is within range 0 to 100
void testInRange(int result[], int len){
  bool passed = true; 
  for (int i = 0; i < len; i++){
    if (result[i] > 100 || result[i] < 0)
      passed = false;
   }

  if(passed)
    Serial.println("RANGE TEST PASSED\n");
  else 
    Serial.println("RANGE TEST FAILED\n");
}

// Make sure brightness is decreasing at an increasing rate 
void testDecInc(int result[], int len){
  bool passed = true; 
   for (int i = 0; i < len - 2; i++){
     if (result[i] - result[i + 1] > result[i + 1] - result[i + 2]){
        // Serial.print("Brightness value is not decreasing at an increasing rate ");
        // Serial.println();
        passed = false;
      }
  }

  if(passed)
    Serial.println("DEC/INC TEST PASSED\n");
  else 
    Serial.println("DEC/INC TEST FAILED\n");
}
void debug(){
  Serial.print("LDR Value: ");
  Serial.println(LDRValue);
  Serial.print("Brightness: ");
  Serial.println(brightness);
}

// OTHER TYPES OF MAPPINGS 

// Linear dimming relative to ambient light 
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

// Log dimming relative to ambient light 
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
