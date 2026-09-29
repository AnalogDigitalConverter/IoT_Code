#ifndef SALIDAS_H
#define SALIDAS_H

#include <Arduino.h>

/*
 * Inicializa las salidas de la aplicación.
 */
void salidas_iniciar();

/*
 * Procesa las salidas después de un cambio validado del pulsador.
 */
void salidas_procesar(uint8_t estadoPulsador, uint8_t contador);

#endif
