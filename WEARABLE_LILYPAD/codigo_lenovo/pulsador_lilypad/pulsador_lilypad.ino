/* PROJECT TREE----------------------------------------------------------------
 * pulsador_lilypad/
 * ├── pulsador_lilypad.ino
 * ├── config.h
 * ├── timer_sw.h
 * ├── timer_sw.cpp
 * ├── pulsador.h
 * ├── pulsador.cpp
 * ├── salidas.h
 * └── salidas.cpp
 */

#include "config.h"
#include "timer_sw.h"
#include "pulsador.h"
#include "salidas.h"

void setup()
{
  pulsador_iniciar();
  salidas_iniciar();
}

void loop()
{
  /*
   * Se actualizan todos los temporizadores con una única lectura
   * de millis().
   */
  timer_sw_actualizar();

  /*
   * Las salidas solo se ejecutan cuando el pulsador ha producido
   * una transición validada.
   */
  if (pulsador_actualizar())
  {
    salidas_procesar(pulsador_estado(), pulsador_contador());
  }
}
