#include "pulsador.h"
#include "config.h"
#include "timer_sw.h"

namespace
{
  uint8_t estadoEstable  = LOW;
  uint8_t ultimaLectura  = LOW;
  uint8_t contador       = 0U;
}

/*
 * Inicializa el pin de entrada y el estado interno del módulo.
 */
void pulsador_iniciar()
{
  pinMode(PIN_PULSADOR, INPUT);

  estadoEstable = LOW;
  ultimaLectura = LOW;
  contador      = 0U;
}

/*
 * Detecta cambios y aplica el antirrebote mediante un temporizador
 * software no bloqueante.
 */
bool pulsador_actualizar()
{
  const uint8_t lectura = digitalRead(PIN_PULSADOR);

  /*
   * Una lectura diferente inicia o reinicia el temporizador.
   * Todavía no se acepta como estado válido.
   */
  if (lectura != ultimaLectura)
  {
    ultimaLectura = lectura;
    timer_sw_iniciar(TIMER_REBOTE, TIEMPO_REBOTE);
  }

  /*
   * La transición se valida únicamente después de permanecer estable
   * durante el intervalo de antirrebote.
   */
  if (timer_sw_leer_flag(TIMER_REBOTE))
  {
    estadoEstable = ultimaLectura;

    /*
     * Solo se cuenta la transición a HIGH.
     */
    if (estadoEstable == HIGH)
    {
      ++contador;
    }

    return true;
  }

  return false;
}

/*
 * Devuelve el último estado estable validado.
 */
uint8_t pulsador_estado()
{
  return estadoEstable;
}

/*
 * Devuelve el número de pulsaciones válidas.
 */
uint8_t pulsador_contador()
{
  return contador;
}
