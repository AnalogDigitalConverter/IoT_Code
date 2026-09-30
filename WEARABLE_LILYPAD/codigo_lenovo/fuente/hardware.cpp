/* hardware.h */
#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

// --- BARRA DE 6 LEDS BLANCOS ---
#define PIN_LED_0 A3  // PF5
#define PIN_LED_1 2   // PD2
#define PIN_LED_2 3   // PD3
#define PIN_LED_3 7   // PE6
#define PIN_LED_4 A4  // PF1
#define PIN_LED_5 A5  // PF0

// --- LED RGB ---
#define PIN_RGB_R 10  // PB6
#define PIN_RGB_G 5   // PC6
#define PIN_RGB_B 13  // PC7

// --- PERIFÉRICOS DE ENTRADA / SALIDA ---
#define PIN_LUMINOSIDAD A2  // Sensor fotosensible
#define PIN_ZUMBADOR    9   // Buzzer
#define PIN_PULSADOR    A1  // Pulsador de apagado de alarma

// --- ACELERÓMETRO ADXL335 (1.3) ---
#define PIN_ACCEL_X A0
#define PIN_ACCEL_Y A8
#define PIN_ACCEL_Z A11

#endif // HARDWARE_H