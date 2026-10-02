#include "hardware.h"

const uint8_t barra_leds[] = {PIN_LED_0, PIN_LED_1, PIN_LED_2, PIN_LED_3, PIN_LED_4, PIN_LED_5};
uint16_t luminosidad = 0;
uint8_t cycle = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_PULSADOR, INPUT_PULLUP); // Conexión con resistencia interna a VCC??
  pinMode(PIN_LUMINOSIDAD, INPUT);
  pinMode(PIN_ACCEL_X, INPUT);
  pinMode(PIN_ACCEL_Y, INPUT);
  pinMode(PIN_ACCEL_Z, INPUT);
  
  pinMode(PIN_ZUMBADOR,     OUTPUT);

  for (int x = 0; x <= 5; x++){
    pinMode(barra_leds[x], OUTPUT);
  }

  pinMode(PIN_RGB_R,        OUTPUT);
  pinMode(PIN_RGB_G,        OUTPUT);
  pinMode(PIN_RGB_B,        OUTPUT);

  /*fsm init*/
  //ESTADO INICIAL OFF

  //debug
  Serial.begin(9600);
  delay(1000);

}


//char [16] texto;

void loop() {
  // put your main code here, to run repeatedly:

  //medir
  luminosidad = medir_luminosidad();
  
  //output
  cycle++;
  Serial.print("cycle: ");
  Serial.println(cycle);

  Serial.print("sensor value: ");
  Serial.println(luminosidad);
  Serial.println("");

 //pintar
  pintar_luminosidad(1023, barra_leds);

  delay (1000);

}

uint16_t medir_luminosidad(void){
  /*SENSOR POLARIZADO A 3.3V, Medida(ADC) entre sensor y resistencia "pull down" de 10k*/
  /*Fijar referencia: analog Reference*/
  /*Determinar LSB (3.3v/1024) y programar función de transferencia*/
  //d=Vin/Vref*1024
  //analogReference(DEFAULT, EXTERNAL)
  //analogReference(DEFAULT);

  return analogRead(PIN_LUMINOSIDAD);
  
}


void pintar_luminosidad(uint8_t luminosidad, uint8_t* barra_leds){

  // Create a LED bargraph using value as an input.
  // Value should be in the range 0 to 1023.

  int x;
  
  // Step through the bargraph LEDs,
  // Turn them on or off depending on value.

  // Value will be in the range 0 to 1023.
  // There are 6 LEDs in the bargraph.
  // 1023 divided by 6 is 170, so 170 will be our threshold
  // between each LED (0,42,84, etc.)

  for (x=0; x <= 5; x++)
  {
    if (luminosidad > (x*170) )
    {
      digitalWrite(barra_leds[x], HIGH);
    }
    else
    {
      digitalWrite(barra_leds[x], LOW);
    }    
  }


}

