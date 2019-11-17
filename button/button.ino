const int buttonPin = 2;
const int ledPin = 9;
int buttonState = 0;
bool buttonON = 0;
byte brightness = 0;
bool pressed = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // sets serial port for communication
  pinMode(ledPin, OUTPUT);
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
    // reverse state of LED
//    buttonON = not buttonON;
    brightness = cycle(brightness);
    pressed = true;
  } 
  else if (buttonState == LOW and pressed) {
    pressed = false;
  }

//   if (buttonON){
//      brightness = 255;
//    }
//   else {
//      brightness = 0;
//    }
   analogWrite(ledPin, brightness);
   //delay(300);

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
