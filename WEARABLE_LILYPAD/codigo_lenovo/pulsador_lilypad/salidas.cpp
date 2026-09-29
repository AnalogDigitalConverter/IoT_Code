#include "salidas.h"
#include "config.h"

  void salida_serie(uint8_t estadoPulsador, uint8_t contador);
  void conmutar_led_cada(uint8_t contador, uint8_t modulo);

#if SALIDA_ANALOG
  void salida_analog(uint8_t contador);
#endif


/*
 * Inicializa las salidas físicas y la comunicación serie.
 */
void salidas_iniciar()
{
  pinMode(PIN_LED, OUTPUT);

#if SALIDA_ANALOG
  pinMode(PIN_PWM_1, OUTPUT);
  pinMode(PIN_PWM_2, OUTPUT);
#endif

  Serial.begin(9600);
}

/*
 * Ejecuta todas las salidas asociadas a un evento válido.
 */
void salidas_procesar(uint8_t estadoPulsador, uint8_t contador)
{
  salida_serie(estadoPulsador, contador);
  conmutar_led_cada(contador, MODULO_LED);

#if SALIDA_ANALOG
  salida_analog(contador);
#endif
}

/*
 * Envía el estado y el contador por el puerto serie.
 */
void salida_serie(uint8_t estadoPulsador, uint8_t contador)
{
  if (estadoPulsador == LOW)
  {
    Serial.println(F("off"));
  }
  else
  {
    Serial.println(F("on"));
    Serial.print(F("number of button pushes: "));
    Serial.println(contador);
  }
}

/*
 * Activa el LED cuando el contador es múltiplo de modulo.
 *
 * La máscara solo es válida si modulo es una potencia de dos:
 *
 *     contador % modulo
 *
 * equivale a:
 *
 *     contador & (modulo - 1)
 */
void conmutar_led_cada(uint8_t contador, uint8_t modulo)
{
  if ((contador & (modulo - 1U)) == 0U)
  {
    digitalWrite(PIN_LED, HIGH);
  }
  else
  {
    digitalWrite(PIN_LED, LOW);
  }
}

#if SALIDA_ANALOG

/*
 * Genera una salida PWM proporcional al contador.
 */
void salida_analog(uint8_t contador)
{
  const uint8_t brillo = ((uint16_t)contador * 255U) >> 2U;

  analogWrite(PIN_PWM_1, brillo);
  analogWrite(PIN_PWM_2, brillo);
}

#endif
