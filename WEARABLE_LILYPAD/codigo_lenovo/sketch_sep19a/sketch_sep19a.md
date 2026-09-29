#define SALIDA_ANALOG   0
#define MODULO_LED      4

// this constant won't change:
const uint8_t buttonPin = 2;        // the pin that the pushbutton is attached to
const uint8_t ledPin    = 13;       // the pin that the LED is attached to

// Variables will change:
uint8_t buttonPushCounter = 0;   // counter for the number of button presses
uint8_t buttonState       = 0;         // current state of the button
uint8_t lastButtonState   = 0;     // previous state of the button
bool cambio;

void detectar_pulsador();
void conmutar_led_cada(uint8_t modulo);
void salida_serie();
void salida_analog();

void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {

  detectar_pulsador();

  /*Cambio en los outputs*/
  if(cambio){
    salida_serie();
    conmutar_led_cada(MODULO_LED);
#if SALIDA_ANALOG
    salida_analog();
#endif
  }

}

/*RUTINAS*/
void detectar_pulsador(){
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState)
  {
    cambio = true;
    if(buttonState == HIGH) buttonPushCounter++;
  }

  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
  delay(50);  //rebotes

}

void salida_serie(){
  if (buttonState == LOW)
    Serial.println("off");
  else
  {
    Serial.println("on");
    Serial.print("number of button pushes: ");
    Serial.println(buttonPushCounter);
  }
}

void conmutar_led_cada(uint8_t mod){
  (buttonPushCounter % mod == 0) ? digitalWrite(ledPin, HIGH) : digitalWrite(ledPin, LOW);
}

void salida_analog(uint8_t cnt){
  uint8_t brillo = (cnt * 255) >> 2;
  analogWrite(5,  brillo);
  analogWrite(6,  brillo);
}
