#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/* Opciones de compilación. */
#define SALIDA_ANALOG  0U
#define MODULO_LED     4U
#define TIEMPO_REBOTE  50U

/*
 * Pines del LilyPad.
 *
 * Estos valores pueden modificarse sin tocar el resto de los módulos.
 */
constexpr uint8_t PIN_PULSADOR = 2U;
constexpr uint8_t PIN_LED      = 13U;
constexpr uint8_t PIN_PWM_1    = 5U;
constexpr uint8_t PIN_PWM_2    = 6U;

/* Identificadores de temporizadores software. */
enum TimerId : uint8_t
{
  TIMER_REBOTE = 0U,
  NUM_TIMERS_SW
};

#endif
