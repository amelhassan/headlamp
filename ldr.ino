#define LDRpin A0 // pin where we connected the LDR and the resistor
int ledPin = 9;
int LDRValue = 0;     // result of reading the analog pin
byte brightness = 255;
byte ramp = 105
bool dimmed = false;

void setup() {
  Serial.begin(9600); // sets serial port for communication
  pinMode(ledPin, OUTPUT);
}

void loop() {
  LDRValue = analogRead(LDRpin); // read the value from the LDR
  info();
  // The higher the LDRValue the brighter the enviornment is 
  // The lower the LDRValue the darker the enviornment is 
  //Serial.println(LDRValue);      // print the value to the serial port, tools -> Serial Monitor 
  delay(100);                    // wait a little
  if (LDRValue > 1000){
      Serial.println("dimming");
      brightness = 105;
      analogWrite(ledPin, brightness); // dims 
    delay(200);
    dimmed = true;
  }
  else{
    if (dimmed) {
      rampUp();
      dimmed = false;
    }
    brightness = 255;
    analogWrite(ledPin, brightness);
    //delay(200);
    }
}

void rampUp(){
  Serial.println("Entered rampup"); 
  info();
  while (brightness < 255){
    info();
      brightness += 50;
      analogWrite(ledPin, brightness);
      delay(1000);
    }
    Serial.println("Exiting rampup");
    return;
 }

void info(){
  Serial.print("LDR: ");
  Serial.print(LDRValue);
  Serial.print('\n');
  Serial.print("Brightness: ");
  Serial.print(brightness);
  Serial.print('\n');
}
