#include "hardware.h"

// --- CONSTANTES EN TICKS (1 tick = 100 ms) ---
#define TICKS_CADENCIA_BARRA    4     // 400 ms / 100 ms
#define TICKS_PARPADEO_RGB      5     // 500 ms / 100 ms
#define TICKS_EXPOSICION_MITAD  100   // 10 s / 0.1 s
#define TICKS_EXPOSICION_TOTAL  200   // 20 s / 0.1 s
#define TICKS_BLOQUEO_ALARMA    300   // 30 s / 0.1 s

#define UMBRAL_LUMINOSIDAD_MAX  700   // Ajustable en compilación
#define ACCEL_Z_MIN_CONSULTA    550   // Umbral de orientación para el ADXL335

const uint8_t barra_leds[6] = {PIN_LED_0, PIN_LED_1, PIN_LED_2, PIN_LED_3, PIN_LED_4, PIN_LED_5};

// ESTADOS DE LA MÁQUINA DE ESTADOS (FSM)
enum EstadoFSM {
  NORMAL,
  PRE_ALARMA,
  ALARMA_ACTIVA,
  REPOSO_POST_ALARMA
};

volatile EstadoFSM estadoActual = NORMAL;

// CONTADORES INCREMENTADOS DENTRO DE LA ISR (TIMER1)
volatile uint16_t ticksExposicion = 0;
volatile uint16_t ticksPostAlarma = 0;
volatile uint8_t  ticksBarra      = 0;
volatile uint8_t  ticksRGB        = 0;

volatile bool flagRefrescarBarra = false;
volatile bool estadoRGBG         = false;

// -------------------------------------------------------------
// INTERRUPCIÓN DE HARDWARE TIMER1 (Ejecutada cada 100 ms)
// -------------------------------------------------------------
ISR(TIMER1_COMPA_vect) {
  
  // 1. Contador para la cadencia de la barra de LEDs (400 ms)
  ticksBarra++;
  if (ticksBarra >= TICKS_CADENCIA_BARRA) {
    ticksBarra = 0;
    flagRefrescarBarra = true;
  }

  // 2. Control según el estado activo de la FSM
  switch (estadoActual) {
    
    case PRE_ALARMA:
      ticksExposicion++;
      
      // Conmutación del LED RGB Verde cada 500 ms (entre 10s y 20s)
      if (ticksExposicion >= TICKS_EXPOSICION_MITAD && ticksExposicion < TICKS_EXPOSICION_TOTAL) {
        ticksRGB++;
        if (ticksRGB >= TICKS_PARPADEO_RGB) {
          ticksRGB = 0;
          estadoRGBG = !estadoRGBG;
          digitalWrite(PIN_RGB_G, estadoRGBG ? HIGH : LOW);
        }
      }

      // Transcurridos 20 s -> Pasar a estado de ALARMA
      if (ticksExposicion >= TICKS_EXPOSICION_TOTAL) {
        digitalWrite(PIN_RGB_G, LOW);
        estadoActual = ALARMA_ACTIVA;
      }
      break;

    case REPOSO_POST_ALARMA:
      ticksPostAlarma++;
      if (ticksPostAlarma >= TICKS_BLOQUEO_ALARMA) {
        ticksPostAlarma = 0;
        estadoActual = NORMAL;
      }
      break;

    default:
      break;
  }
}

// -------------------------------------------------------------
// CONFIGURACIÓN DE TIMERS POR REGISTROS (ATmega32U4)
// -------------------------------------------------------------
void initTimer1_FSM() {
  // Timer1 de 16 bits en modo CTC (Clear Timer on Compare Match)
  // Frecuencia objetivo = 10 Hz (100 ms)
  // OCR1A = (16MHz / (Prescaler * Freq)) - 1 = (16,000,000 / (256 * 10)) - 1 = 6249
  
  cli(); // Desactivar interrupciones globales durante configuración
  
  TCCR1A = 0; 
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 6249;             // Compare Match Value para 100 ms
  TCCR1B |= (1 << WGM12);   // Activar modo CTC
  TCCR1B |= (1 << CS12);    // Prescaler 256
  TIMSK1 |= (1 << OCIE1A);  // Habilitar interrupción Output Compare A

  sei(); // Reorganizar e interrupciones globales habilitadas
}

void setup() {
  // Configuración de salidas
  for (uint8_t i = 0; i < 6; i++) {
    pinMode(barra_leds[i], OUTPUT);
    digitalWrite(barra_leds[i], LOW);
  }
  
  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);
  digitalWrite(PIN_RGB_R, LOW);
  digitalWrite(PIN_RGB_G, LOW);
  digitalWrite(PIN_RGB_B, LOW);

  pinMode(PIN_ZUMBADOR, OUTPUT);

  // Configuración de entradas
  pinMode(PIN_PULSADOR, INPUT_PULLUP);
  pinMode(PIN_LUMINOSIDAD, INPUT);
  pinMode(PIN_ACCEL_X, INPUT);
  pinMode(PIN_ACCEL_Y, INPUT);
  pinMode(PIN_ACCEL_Z, INPUT);

  analogReference(DEFAULT);

  // Inicializar Timer1 para el tick de 100ms de la FSM
  initTimer1_FSM();
}

// -------------------------------------------------------------
// BUCLE PRINCIPAL (EVALUACIÓN DE EVENTOS Y ENTRADAS)
// -------------------------------------------------------------
void loop() {
  uint16_t nivelLuz = analogRead(PIN_LUMINOSIDAD);
  bool pulsadorPresionado = (digitalRead(PIN_PULSADOR) == LOW);

  // Control principal de la FSM
  switch (estadoActual) {

    case NORMAL:
      if (nivelLuz >= UMBRAL_LUMINOSIDAD_MAX) {
        ticksExposicion = 0;
        ticksRGB = 0;
        estadoActual = PRE_ALARMA;
      }
      break;

    case PRE_ALARMA:
      // Reseteo si baja la intensidad de luz
      if (nivelLuz < UMBRAL_LUMINOSIDAD_MAX) {
        digitalWrite(PIN_RGB_G, LOW);
        ticksExposicion = 0;
        estadoActual = NORMAL;
      }
      break;

    case ALARMA_ACTIVA:
      // Activa el zumbador sin bloquear el procesador
      tone(PIN_ZUMBADOR, 523); // Nota C5

      if (pulsadorPresionado) {
        noTone(PIN_ZUMBADOR);
        ticksPostAlarma = 0;
        estadoActual = REPOSO_POST_ALARMA;
      }
      break;

    case REPOSO_POST_ALARMA:
      break;
  }

  // Refresco no bloqueante de la barra LED según el acelerómetro (cada 400 ms)
  if (flagRefrescarBarra) {
    flagRefrescarBarra = false;
    
    if (detectarGestoConsulta()) {
      actualizarBarraLeds(nivelLuz);
    } else {
      apagarBarraLeds();
    }
  }
}

// -------------------------------------------------------------
// FUNCIONES AUXILIARES
// -------------------------------------------------------------
bool detectarGestoConsulta() {
  return (analogRead(PIN_ACCEL_Z) > ACCEL_Z_MIN_CONSULTA);
}

void actualizarBarraLeds(uint16_t luz) {
  uint8_t ledsAEncender = map(luz, 0, UMBRAL_LUMINOSIDAD_MAX, 0, 6);
  ledsAEncender = constrain(ledsAEncender, 0, 6);

  for (uint8_t i = 0; i < 6; i++) {
    digitalWrite(barra_leds[i], (i < ledsAEncender) ? HIGH : LOW);
  }
}

void apagarBarraLeds() {
  for (uint8_t i = 0; i < 6; i++) {
    digitalWrite(barra_leds[i], LOW);
  }
}