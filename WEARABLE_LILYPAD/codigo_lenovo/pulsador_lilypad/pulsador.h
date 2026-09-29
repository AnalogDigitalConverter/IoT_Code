#ifndef PULSADOR_H
#define PULSADOR_H

#include <Arduino.h>

/*
 * Inicializa el módulo del pulsador.
 */
void pulsador_iniciar();

/*
 * Actualiza el estado del pulsador.
 *
 * Devuelve true cuando se ha validado un cambio de estado después
 * del tiempo de antirrebote.
 */
bool pulsador_actualizar();

/*
 * Devuelve el estado estable actual del pulsador.
 */
uint8_t pulsador_estado();

/*
 * Devuelve el número de pulsaciones válidas.
 */
uint8_t pulsador_contador();

#endif
