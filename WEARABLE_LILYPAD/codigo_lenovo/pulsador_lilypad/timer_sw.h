#ifndef TIMER_SW_H
#define TIMER_SW_H

#include <Arduino.h>

/*
 * Inicializa o reinicia un temporizador software.
 *
 * La flag asociada se borra al iniciar el temporizador.
 */
void timer_sw_iniciar(uint8_t id, uint16_t periodo);

/*
 * Actualiza todos los temporizadores activos.
 *
 * Debe llamarse periódicamente desde loop().
 */
void timer_sw_actualizar();

/*
 * Lee y consume la flag de un temporizador.
 *
 * Devuelve true una sola vez por cada expiración.
 */
bool timer_sw_leer_flag(uint8_t id);

#endif
