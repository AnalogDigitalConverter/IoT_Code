#include hardware.h

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_PULSADOR,     INPUT);
  pinMode(PIN_LUMINOSIDAD,  INPUT);
  pinMode(PIN_ZUMBADOR,     OUTPUT);
  pinMode(PIN_LED_0,        OUTPUT);
  pinMode(PIN_LED_1,        OUTPUT);
  pinMode(PIN_LED_2,        OUTPUT);
  pinMode(PIN_LED_3,        OUTPUT);
  pinMode(PIN_LED_4,        OUTPUT);
  pinMode(PIN_LED_5,        OUTPUT);
  pinMode(PIN_LED_R,        OUTPUT);
  pinMode(PIN_LED_G,        OUTPUT);
  pinMode(PIN_LED_B,        OUTPUT);
  /*fsm init*/
  //ESTADO INICIAL OFF

}

const uint8_t barra_leds[] = {PIN_LED_5, PIN_LED_4, PIN_LED_3, PIN_LED_2, PIN_LED_1, PIN_LED_0};
uint8_t luminosidad = 0;

void loop() {
  // put your main code here, to run repeatedly:

  while (estado = activo){
  //medir
  luminosidad = medir_luminosidad(PIN_LUMINOSIDAD);
  //pintar
  pintar_luminosidad(luminosidad, barra_leds);

  //sonar -> esto jode el timing en tiempo de ejecución
  tone(PIN_ZUMBADOR,NOTE_C5);
  delay(200);
  noTone(PIN_ZUMBADOR);
  delay(800);

  //parpadear
  parpadear();
  }



}

void pintar_luminosidad(uint8_t luminosidad, uint8_t* barra_leds){

  for (int i = 0; i < 6; i++){
    digitalWrite(barra_leds[i],LOW); // If buttonState is HIGH (unpressed), turn off the LED

  }


}
