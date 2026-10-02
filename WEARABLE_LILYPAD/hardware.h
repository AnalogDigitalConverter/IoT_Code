#ifndef HARDWARE_H
#define HARDWARE_H

// --- BARRA DE 6 LEDS BLANCOS ---
#define PIN_LED_0 20  // F5
#define PIN_LED_1 19  // D2
#define PIN_LED_2 18  // D3
#define PIN_LED_3 17  // E6
#define PIN_LED_4 16  // F1
#define PIN_LED_5 15  // F0

// --- LED RGB ---
#define PIN_RGB_R 10  // B6
#define PIN_RGB_G 5   // C6
#define PIN_RGB_B 13  // C7

// --- SENSOR DE LUMINOSIDAD Y BUZZER ---
#define PIN_LUMINOSIDAD A2  // Sensor fotosensible
#define PIN_ZUMBADOR    9   // Buzzer piezoeléctrico
#define PIN_PULSADOR    A1  // Pulsador (configurado con INPUT_PULLUP)

// --- ACELERÓMETRO ADXL335 (1.3) ---
#define PIN_ACCEL_X A0
#define PIN_ACCEL_Y A8
#define PIN_ACCEL_Z A11


#endif
