#include hardware.h

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_PULSADOR, INPUT_PULLUP); // Conexión con resistencia interna a VCC??
  pinMode(PIN_LUMINOSIDAD, INPUT);
  pinMode(PIN_ACCEL_X, INPUT);
  pinMode(PIN_ACCEL_Y, INPUT);
  pinMode(PIN_ACCEL_Z, INPUT);
  
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

const uint8_t barra_leds[6] = {PIN_LED_0, PIN_LED_1, PIN_LED_2, PIN_LED_3, PIN_LED_4, PIN_LED_5};
uint16_t luminosidad = 0;

void loop() {
  // put your main code here, to run repeatedly:

  while (estado = activo){
  //medir
  luminosidad = medir_luminosidad();
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

uint16_t medir_luminosidad(void){
  /*SENSOR POLARIZADO A 3.3V, Medida(ADC) entre sensor y resistencia "pull down" de 10k*/
  /*Fijar referencia: analog Reference*/
  /*Determinar LSB (3.3v/1024) y programar función de transferencia*/
  //d=Vin/Vref*1024
  //analogReference(DEFAULT, EXTERNAL)
  analogReference(DEFAULT);
  return analogRead(PIN_LUMINOSIDAD);
  
}


void pintar_luminosidad(uint8_t luminosidad, uint8_t* barra_leds){

  for (int i = 0; i < 6; i++){
    digitalWrite(barra_leds[i],LOW); // If buttonState is HIGH (unpressed), turn off the LED

  }


}
