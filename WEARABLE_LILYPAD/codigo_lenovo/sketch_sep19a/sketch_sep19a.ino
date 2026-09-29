/*
 * -----------------------------------------------------------------------------
 * @file    pulsador_timers.ino
 * @brief   Detección de pulsador con antirrebote mediante temporizador software.
 *
 * El programa detecta cambios de estado en un pulsador, cuenta las pulsaciones
 * válidas y actualiza las salidas asociadas al evento.
 *
 * Los temporizadores software se basan en millis() y no bloquean la ejecución
 * mediante delay().
 * -----------------------------------------------------------------------------
 */

#define SALIDA_ANALOG   0U
#define MODULO_LED      4U
#define TIEMPO_REBOTE   50U

#define NUM_TIMERS_SW   1U
#define TIMER_REBOTE    0U

/* Identificación de las entradas y salidas utilizadas. */
const uint8_t buttonPin = 2U;
const uint8_t ledPin    = 13U;

/* Estado funcional del pulsador. */
static uint8_t buttonPushCounter = 0U;
static uint8_t buttonState       = LOW;
static uint8_t lastButtonState   = LOW;
static bool cambio               = false;

/*
 * Estado de un temporizador software.
 *
 * inicio  : instante de activación obtenido mediante millis().
 * periodo : tiempo de espera expresado en milisegundos.
 * activo  : indica que el temporizador está contando.
 * flag    : indica que el temporizador ha expirado.
 */
typedef struct
{
  uint32_t inicio;
  uint16_t periodo;
  bool activo;
  bool flag;
} TimerSW;

/* Tabla de temporizadores software disponibles. */
static TimerSW timers[NUM_TIMERS_SW];

/* Prototipos de la gestión de temporizadores software. */
void iniciar_timer_sw(uint8_t id, uint16_t tiempo);
void actualizar_timers_sw();
bool leer_flag_timer_sw(uint8_t id);

/* Prototipos de la aplicación. */
void detectar_pulsador();
void conmutar_led_cada(uint8_t modulo);
void salida_serie();
void salida_analog(uint8_t cnt);

/*
 * @brief Inicializa los periféricos utilizados por la aplicación.
 */
void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

/*
 * @brief Ejecuta el ciclo principal de la aplicación.
 *
 * La actualización de todos los temporizadores se realiza una sola vez
 * por ciclo y utiliza una única lectura de millis().
 */
void loop()
{
  actualizar_timers_sw();

  detectar_pulsador();

  /*
   * Las salidas solo se actualizan cuando se ha validado un cambio
   * de estado del pulsador.
   */
  if (cambio)
  {
    salida_serie();
    conmutar_led_cada(MODULO_LED);

#if SALIDA_ANALOG
    salida_analog(buttonPushCounter);
#endif
  }
}

/*
 * @brief Detecta y valida cambios de estado del pulsador.
 *
 * Una variación de la lectura inicia o reinicia el temporizador de
 * antirrebote. El nuevo estado solo se acepta cuando permanece estable
 * durante TIEMPO_REBOTE milisegundos.
 */
void detectar_pulsador()
{
  const uint8_t lectura = digitalRead(buttonPin);

  /* Se presupone que no existe un evento nuevo en este ciclo. */
  cambio = false;

  /*
   * Una lectura diferente indica una posible transición.
   * El temporizador se reinicia para descartar rebotes sucesivos.
   */
  if (lectura != lastButtonState)
  {
    lastButtonState = lectura;
    iniciar_timer_sw(TIMER_REBOTE, TIEMPO_REBOTE);
  }

  /*
   * La transición se considera válida únicamente después de finalizar
   * el intervalo de antirrebote.
   */
  if (leer_flag_timer_sw(TIMER_REBOTE))
  {
    buttonState = lastButtonState;
    cambio = true;

    /*
     * Solo se contabiliza la transición a nivel alto.
     * La transición a nivel bajo no incrementa el contador.
     */
    if (buttonState == HIGH)
    {
      ++buttonPushCounter;
    }
  }
}

/*
 * @brief Envía por el puerto serie el estado del pulsador y el contador.
 */
void salida_serie()
{
  if (buttonState == LOW)
  {
    Serial.println(F("off"));
  }
  else
  {
    Serial.println(F("on"));
    Serial.print(F("number of button pushes: "));
    Serial.println(buttonPushCounter);
  }
}

/*
 * @brief Activa el LED en múltiplos del módulo indicado.
 *
 * La máscara de bits sustituye al operador módulo cuando modulo es una
 * potencia de dos. En ese caso:
 *
 *     contador % modulo
 *
 * equivale a:
 *
 *     contador & (modulo - 1)
 *
 * Esta función requiere que modulo sea 2, 4, 8, 16, etc.
 */
void conmutar_led_cada(uint8_t modulo)
{
  if ((buttonPushCounter & (modulo - 1U)) == 0U)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
}

/*
 * @brief Genera la salida PWM proporcional al contador.
 *
 * La multiplicación se realiza en 16 bits para evitar desbordamientos
 * durante el cálculo intermedio.
 */
void salida_analog(uint8_t cnt)
{
  const uint8_t brillo = ((uint16_t)cnt * 255U) >> 2U;

  analogWrite(5U, brillo);
  analogWrite(6U, brillo);
}

/*
 * @brief Inicia o reinicia un temporizador software.
 *
 * @param id      Índice del temporizador que se desea iniciar.
 * @param tiempo  Duración del temporizador en milisegundos.
 *
 * La flag asociada se borra al iniciar el temporizador. Una nueva
 * expiración volverá a activarla.
 */
void iniciar_timer_sw(uint8_t id, uint16_t tiempo)
{
  if (id >= NUM_TIMERS_SW)
  {
    return;
  }

  timers[id].inicio  = millis();
  timers[id].periodo = tiempo;
  timers[id].activo  = true;
  timers[id].flag    = false;
}

/*
 * @brief Actualiza todos los temporizadores software activos.
 *
 * La resta entre valores unsigned de millis() funciona correctamente
 * aunque millis() se desborde y vuelva a cero:
 *
 *     ahora - timers[id].inicio
 *
 * Por este motivo se compara el tiempo transcurrido y no una hora final.
 *
 * La aritmética modular de uint32_t permite esta técnica para intervalos
 * inferiores a la mitad del rango representable por uint32_t.
 */
void actualizar_timers_sw()
{
  const uint32_t ahora = millis();

  for (uint8_t id = 0U; id < NUM_TIMERS_SW; ++id)
  {
    TimerSW &timer = timers[id];

    if (timer.activo &&
        ((uint32_t)(ahora - timer.inicio) >= timer.periodo))
    {
      timer.activo = false;
      timer.flag   = true;
    }
  }
}

/*
 * @brief Lee y consume la flag de un temporizador.
 *
 * @param id Índice del temporizador cuya flag se desea consultar.
 *
 * @return true  si el temporizador ha expirado desde la última lectura.
 * @return false si no existe una expiración pendiente.
 *
 * La flag se borra al leerla, por lo que cada expiración genera un único
 * evento para el código de aplicación.
 */
bool leer_flag_timer_sw(uint8_t id)
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
