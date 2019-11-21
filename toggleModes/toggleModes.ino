const int buttonPin = 2;
const int ledPinBlue = 9;
const int ledPinRed = 8;
int buttonState = 0;
bool buttonON = 0;
byte brightness = 0;
bool pressed = false;
int bRed = 0;
int bBlue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // sets serial port for communication
  pinMode(ledPinBlue, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  pinMode(buttonPin, INPUT);

}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  Serial.print("Button value: ");
  Serial.print(buttonState);
  Serial.print("\n");
  
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH and not pressed) {
    cycle();
    pressed = true;
  } 
  else if (buttonState == LOW and pressed) {
    pressed = false;
  }

   analogWrite(ledPinBlue, bBlue);
   analogWrite(ledPinRed, bRed);
   

}

byte cycle( ) {
  if (bBlue == 0 and bRed == 0) {
    bRed= 100; // brightness for the red light 
  } 
  else if (bBlue == 0 and bRed == 100){ 
    bRed = 0;
    bBlue = 10;
  }
  else if (bBlue == 10 and bRed == 0) {
    bBlue = 255;
  } 
  else if (bBlue == 255 and bRed == 0) {
    bBlue = 0;
  }
  
}
