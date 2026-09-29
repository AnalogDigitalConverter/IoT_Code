#include "timer_sw.h"
#include "config.h"

namespace
{
  struct Timer
  {
    uint32_t inicio;
    uint16_t periodo;
    bool activo;
    bool flag;
  };

  Timer timers[NUM_TIMERS_SW] = {};
}

/*
 * Inicia o reinicia un temporizador.
 */
void timer_sw_iniciar(uint8_t id, uint16_t periodo)
{
  if (id >= NUM_TIMERS_SW)
  {
    return;
  }

  timers[id].inicio  = millis();
  timers[id].periodo = periodo;
  timers[id].activo  = true;
  timers[id].flag    = false;
}

/*
 * Actualiza todos los temporizadores activos.
 *
 * La resta entre valores unsigned de millis() continúa funcionando
 * correctamente cuando millis() se desborda y vuelve a cero:
 *
 *     ahora - inicio
 *
 * Por ello se compara el tiempo transcurrido y no una hora final.
 *
 * Esta técnica es válida para intervalos inferiores a la mitad del
 * rango de uint32_t.
 */
void timer_sw_actualizar()
{
  const uint32_t ahora = millis();

  for (uint8_t id = 0U; id < NUM_TIMERS_SW; ++id)
  {
    Timer &timer = timers[id];

    if (timer.activo &&
        ((uint32_t)(ahora - timer.inicio) >= timer.periodo))
    {
      timer.activo = false;
      timer.flag   = true;
    }
  }
}

/*
 * Lee y consume la flag de un temporizador.
 */
bool timer_sw_leer_flag(uint8_t id)
{
  if (id >= NUM_TIMERS_SW)
  {
    return false;
  }

  if (timers[id].flag)
  {
    timers[id].flag = false;
    return true;
  }

  return false;
}
