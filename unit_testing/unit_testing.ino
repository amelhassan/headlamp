#define LDRpin A0 // pin where we connected the LDR and the resistor
#define LED_PIN A4
#define LED_STATE_CHANGE A3 // digital yellow light to indicate when button is pressed
#define BUTTON_PIN 5
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // sets serial port for communication
  pinMode(LDRpin, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_STATE_CHANGE, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  // turn white LED on 
  analogWrite(LED_PIN, 255);

}

void loop() {
  // put your main code here, to run repeatedly:

  // print when button if button is pressed or not 
  Serial.print("Button state: ");
  Serial.println(digitalRead(BUTTON_PIN));

  // Output ldr value 
  Serial.print("LDR value: ");
  Serial.println(analogRead(LDRpin));

  delay(100);

}
