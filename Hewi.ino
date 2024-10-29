#include <samplerSynth.h>       // Biblioteca para sintetizador de audio en el ESP32
#include <MPU6050_light.h>       // Biblioteca para el sensor MPU6050 (giroscopio/acelerómetro)
#include "Wire.h"                // Biblioteca para comunicación I2C
#include <Breath.h>              // Biblioteca para manejar el sensor de soplo
#include <BLEMidi.h>             // Biblioteca para MIDI a través de Bluetooth Low Energy
#include <Preferences.h>         // Biblioteca para almacenar configuraciones de usuario en la memoria no volátil
#include <sm.h>                  // Biblioteca personalizada para crear un menú simple
#include <tb.h>                  // Biblioteca personalizada para el uso de pines táctiles capacitivos
//---------------------------------------------------------------------------------------------------------------------------------------------
// Configuración inicial de constantes y macros
#define LIBRARY 0                // Define si se usa U8g2lib (0) o Adafruit_GFX (1) para el display
#define NOMBRE "HEWI 0.9"        // Nombre de la versión del instrumento
#define MSG_TIME 700             // Tiempo de visualización de mensajes en pantalla
#define UMBRAL 20                // Umbral de activación para el sensor de soplido
#define TOUCH_UMBRAL 60          // Umbral de sensibilidad para los pines táctiles
#define interno 0                // Valor para la salida de sonido interna
#define blemidi 1                // Valor para salida MIDI por BLE
#define nota -1                  // Identificador para notas individuales
#define aMayor 0                 // Tipo de acorde mayor
#define aMenor 1                 // Tipo de acorde menor
#define aAume 2                  // Tipo de acorde aumentado
#define aDism 3                  // Tipo de acorde disminuido
//---------------------------------------------------------------------------------------------------------------------------------------------
// Configuración de la pantalla y el menú
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE); // Inicialización del display OLED 0.96" 128x64
sm Menu(display); // Definición del menú
Breath myBreath(5, 18); // Configuración del sensor de soplido en los pines GPIO 5 y 18
MPU6050 mpu(Wire);       // Inicialización del MPU6050 con comunicación I2C
Preferences preferencias; // Objeto para gestionar las preferencias del usuario
tb key1(0, TOUCH_UMBRAL); // Configuración de pin táctil con umbral ajustado
tb key2(2, TOUCH_UMBRAL);
tb key3(3, TOUCH_UMBRAL);
tb key4(4, TOUCH_UMBRAL);
tb key5(5, TOUCH_UMBRAL);
tb key6(6, TOUCH_UMBRAL+10);  // Aumentando el umbral en este pin para evitar falsas lecturas
tb key7(7, TOUCH_UMBRAL+10);
tb key8(8, TOUCH_UMBRAL+10);
//---------------------------------------------------------------------------------------------------------------------------------------------
// Definición de los elementos de menú para navegar las opciones del instrumento
// Menú principal con las opciones base del instrumento
txtMenu Main[] = {
  "Menu",             // Encabezado del menú
  "Tocar",            // Modo para tocar el instrumento
  "Instrumentos",     // Selección del instrumento
  "Volumen",          // Ajuste del volumen
  "Efectos",          // Acceso a los efectos de sonido
  "Filtros",          // Configuración de filtros de sonido
  "Configurar"        // Menú de configuración general
};
// Menú de selección de instrumentos
txtMenu * Instrumentos;  // Puntero para el menú de instrumentos, dinámicamente configurado

// Submenú para la opción "Volumen" en el menú principal
txtMenu Volumen[] = {
  "Volumen",          // Encabezado del menú de volumen
  "Establecer",       // Ajustar el nivel de volumen
  "Salir"             // Salir del submenú de volumen
};
// Submenú de configuración general del instrumento
txtMenu Config[] = {
  "Configurar",       // Encabezado del menú de configuración
  "Sonido",           // Opciones de salida de sonido (interno o BLE-MIDI)
  "Transponer",       // Transposición de notas (cambio de tono en semitonos)
  "Modo Acorde",      // Selección del modo de acordes
  "Sensibilidad",     // Ajuste de la sensibilidad de los sensores
  "Modo Sensor",      // Configuración de modos de sensor (como pitch bend o vibrato)
  "Ver notas",        // Habilitar o deshabilitar la visualización de notas en pantalla
  "Reset",            // Opción para restablecer configuraciones de fábrica
  "Salir"             // Salir del menú de configuración
};
// Opciones para la selección del tipo de sonido en el submenú "Sonido"
txtMenu Sonido[] = {
  "Sonido",           // Encabezado del menú de sonido
  "Interno",          // Sonido emitido desde el propio dispositivo
  "Ble-Midi",         // Sonido a través de Bluetooth MIDI
  "Salir"             // Salir del submenú de sonido
};
// Opciones de efectos de sonido en el submenú "Efectos"
txtMenu Efectos[] = {
  "Efectos",          // Encabezado del menú de efectos
  "Reverberación",    // Activar/desactivar el efecto de reverberación
  "Corus",            // Activar/desactivar el efecto de coro
  "Tremolo",          // Activar/desactivar el efecto de trémolo
  "Salir"             // Salir del submenú de efectos
};
// Opciones para los filtros de sonido en el submenú "Filtros"
txtMenu Filtros[] = {
  "Filtros",          // Encabezado del menú de filtros
  "Paso Bajo",        // Activar/desactivar el filtro de paso bajo
  "Fir",              // Filtro FIR (Respuesta al impulso finito)
  "Salir"             // Salir del submenú de filtros
};
// Opciones de modo de sensor en el submenú "Modo Sensor"
txtMenu ModoSensor[] = {
  "Modo Sensor",      // Encabezado del menú de modo de sensor
  "Pitch Bend",       // Activar el modo de pitch bend (desviación de tono)
  "Vibrato",          // Activar el modo de vibrato (variación periódica de tono)
  "Salir"             // Salir del submenú de modo de sensor
};
//---------------------------------------------------------------------------------------------------------------------------------------------
// Variables de configuración y estado del instrumento musical EWI
// Enumeración para los modos del instrumento
enum { menu, tocar } modo;   // Define dos modos: "menu" y "tocar"

// Mapa de digitaciones a notas MIDI
uint8_t noteMap[256];        // Array para almacenar las combinaciones de digitación y sus notas MIDI correspondientes

// Instrumentos y acordes
uint8_t totalInstr = 0;      // Total de instrumentos disponibles
uint8_t tipoAcordes[4][2] = {{4, 7}, {3, 7}, {4, 8}, {3, 6}}; // Definición de acordes:
                                                              // aMayor (4,7), aMenor (3,7), aAume (4,8), aDism (3,6)
uint8_t instrument;           // Instrumento seleccionado actualmente

// Estado de las teclas táctiles capacitivas
bool key[8];                  // Array para almacenar el estado de las 8 teclas táctiles (abierto o cerrado)

// Velocidad de la nota (intensidad)
uint8_t velocity;             // Velocidad de la nota (intensidad) predeterminada, ajustable por el usuario
uint8_t prevVelocity;         // Velocidad de la nota anterior, utilizada para detectar cambios y minimizar eventos redundantes

// Variables de ajuste de notas y canales MIDI
int8_t canal = 0;             // Canal MIDI actual (rango de 0 a 2)
int8_t modoAcorde = nota;     // Modo de acordes actual, define si es una nota individual o un acorde específico
int8_t prevCanal = 0;         // Canal MIDI anterior para comparar y minimizar cambios innecesarios

// Desplazamiento de notas (transposición)
int8_t transpose;             // Valor de transposición para ajustar el tono de la nota en intervalos de semitono

// Configuración y estado de visualización de notas
bool verNotas;                // Indica si se muestran las notas en pantalla
bool activeBend = 0;          // Controla si el pitch bend (variación de tono) está activo

// Valores para pitch bend y control de expresión
uint8_t bend;                 // Valor para el nivel de pitch bend o expresión actual
uint8_t prevBend;             // Valor de bend anterior para minimizar eventos de control innecesarios

// Almacenamiento del estado de las teclas táctiles
uint8_t finalTouchStates;     // Variable final que representa el estado de las teclas, se envía al "note hopper"

// Variables para el sensor de soplido
int16_t readBreathValue;      // Valor de lectura actual del sensor de soplido
bool finalBreathState;        // Estado final del soplido (activado/desactivado)
bool prevBreathState;         // Estado anterior del soplido, utilizado para enviar eventos de apagado de nota

// Umbral de activación del soplido
int16_t breathBaseVal;        // Nivel base del soplido (valor neutro)
int16_t breathActiveThreshold; // Umbral calculado para detectar activación del soplido

// Variables de la nota activa
uint8_t activePitch;          // Nota MIDI activa actual utilizada para enviar eventos de "noteOn"
uint8_t prevActivePitch;      // Nota MIDI anterior, utilizada para apagar la nota en "noteOff"

// Tiempo de ejecución
unsigned long programTime;    // Tiempo actual de ejecución del programa
unsigned long prevProgramTime;// Tiempo de ejecución anterior, para comparar en eventos

// Estado de la conexión Bluetooth
bool bleConnected;            // Estado de conexión BLE (conectado/desconectado)
bool isBleOn;                 // Indica si BLE está activado o no
// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// Configuración del menú de selección de instrumentos
void setMenuInstrumentos()
{
  // Calcula el número de instrumentos almacenados en memoria de programa (_instrumento)
  uint8_t nProgmemInstr = sizeof(_instrumento) / sizeof(_instrumento[0]);
  
  // Calcula el número de archivos de instrumentos en la memoria (WAV) disponibles
  uint8_t nroWavs = countFiles();
  
  // Calcula el total de instrumentos sumando los de la memoria de programa, los WAV y añade 2 extras (nombre "Instrumentos" y "Salir")
  totalInstr = nProgmemInstr + nroWavs + 2;
  
  // Crea un array dinámico para el menú de instrumentos basado en el total calculado
  Instrumentos = new txtMenu[totalInstr];
  Instrumentos[0] = "Instrumentos"; // Primer elemento del menú, título

  // Rellena el menú con los nombres de instrumentos almacenados en memoria de programa
  for(uint8_t n = 0; n < nProgmemInstr; n++)
  {
    Instrumentos[n + 1] = _instrumento[n].nombre; // Asigna cada nombre al menú
  }

  // Rellena el menú con los nombres de instrumentos WAV en la memoria
  for(uint8_t n = 0; n < nroWavs; n++)
  {
    Instrumentos[n + nProgmemInstr + 1] = _wavInstr[n]; // Asigna cada nombre de archivo WAV al menú
  }

  // Última opción del menú de instrumentos, para salir
  Instrumentos[nProgmemInstr + nroWavs + 1] = "Salir ";
}
//---------------------------------------------------------------------------------------------------------------------------------------------
// Mapeo de digitaciones a notas MIDI, utilizando combinaciones binarias de pulsaciones en los sensores capacitivos
void setNoteMap()
{
 // Mapeo de combinaciones binarias (digitaciones) a notas MIDI
  memset(noteMap, 0, 256);    // Inicializa el array con 0 para evitar notas no deseadas
  noteMap[0b00000000] = 0;  // Sin digitación
  noteMap[0b11111111] = 60; // C4  - ●|●●●|●●●●  - Digitación completa
  noteMap[0b11111011] = 61; // C#4 - ●|●●●|●○●●
  noteMap[0b11111110] = 62; // D4  - ●|●●●|●●●○
  noteMap[0b11111101] = 63; // D#4 - ●|●●●|●●○●
  noteMap[0b11111100] = 64; // E4  - ●|●●●|●●○○
  noteMap[0b11111000] = 65; // F4  - ●|●●●|●○○○
  noteMap[0b11110100] = 66; // F#4 - ●|●●●|○●○○
  noteMap[0b11110000] = 67; // G4  - ●|●●●|○○○○
  noteMap[0b11101000] = 68; // G#4 - ●|●●○|●○○○
  noteMap[0b11100000] = 69; // A4  - ●|●●○|○○○○
  noteMap[0b11010000] = 70; // A#4 - ●|●○●|○○○○
  noteMap[0b11000000] = 71; // B4  - ●|●○○|○○○○
  noteMap[0b10100000] = 72; // C5  - ●|○●○|○○○○
  noteMap[0b10000000] = 73; // C#5 - ●|○○○|○○○○
  noteMap[0b00100000] = 74; // D5  - ○|○●○|○○○○
  noteMap[0b01111100] = 76; // E5  - ○|●●●|●●○○
  noteMap[0b01111000] = 77; // F5  - ○|●●●|●○○○
  noteMap[0b01110100] = 78; // F#5 - ○|●●●|○●○○
  noteMap[0b01110000] = 79; // G5  - ○|●●●|○○○○
  noteMap[0b01101000] = 80; // G#5 - ○|●●○|●○○○
  noteMap[0b01100000] = 81; // A5  - ○|●●○|○○○○
  noteMap[0b01010000] = 82; // A#5 - ○|●○●|○○○○
  noteMap[0b01000000] = 83; // B5  - ○|●○○|○○○○
  noteMap[0b01000001] = 254;//menu - ○|●○○|○○○● combinación especial para acceder al menú (Temporalmente)
}
//---------------------------------------------------------------------------------------------------------------------------------------------
// Lectura de los estados de los sensores capacitivos y conversión a un byte de 8 bits para simplificar el procesamiento
void readKeys()
{
  key[0] = key1.isTouched();  // Lee el estado del sensor capacitivo 1
  key[1] = key2.isTouched();  // Lee el estado del sensor capacitivo 2
  key[2] = key3.isTouched();  // Continúa leyendo hasta el sensor 8
  key[3] = key4.isTouched();
  key[4] = key5.isTouched();
  key[5] = key6.isTouched();
  key[6] = key7.isTouched();
  key[7] = key8.isTouched();
    // Convierte el estado de las teclas en un solo byte de 8 bits (1 bit por tecla) para manejar el mapeo de notas de forma eficiente
  finalTouchStates = key[7] << 7 | key[6] << 6 | key[5] << 5 | key[4] << 4 | key[3] << 3 | key[2] << 2 | key[1] << 1 | key[0];
}
//---------------------------------------------------------------------------------------------------------------------------------------------
// Función para leer el valor del sensor de soplido y determinar si se ha alcanzado el umbral de activación para tocar una nota
bool readBreath()
{
  velocity = readBreathValue = myBreath.read(); // Lee el valor del sensor de soplido y ajusta la velocidad (intensidad) de la nota

  // Si el valor del soplido supera el umbral y no estaba previamente activo, activa el estado de soplido
  if(readBreathValue > breathActiveThreshold && !finalBreathState)
  {
    finalBreathState = HIGH;   // Activa el estado de soplido
  }
  // Si el valor del soplido cae por debajo del umbral y estaba previamente activo, desactiva el estado de soplido
  else if(readBreathValue < breathActiveThreshold && finalBreathState)
  {
    finalBreathState = LOW;    // Desactiva el estado de soplido
  }
  return finalBreathState;     // Devuelve el estado actual del soplido
}
//---------------------------------------------------------------------------------------------------------------------------------------------
// Función para activar (encender) una o más notas en el canal especificado, considerando acordes y conectividad BLE
void notasOn(uint8_t Canal, uint8_t rNota, uint8_t velocidad)
{
  // Si el dispositivo no está conectado por Bluetooth
  if(!bleConnected)
  {
    notaOn(Canal, rNota, velocidad, 0); // Enviar la nota a través del canal especificado
    // Si el modo de acorde está activado, agrega las notas correspondientes al acorde seleccionado
    if(modoAcorde > nota)
    {
      notaOn(Canal, rNota + tipoAcordes[modoAcorde][0], velocidad, 0); // Primera nota del acorde
      notaOn(Canal, rNota + tipoAcordes[modoAcorde][1], velocidad, 0); // Segunda nota del acorde
    }
  }
  else  // Si el dispositivo está conectado por Bluetooth
  {
    BLEMidiServer.noteOn(Canal, rNota, velocidad); // Enviar la nota vía BLE MIDI
    // Enviar las notas del acorde a través de BLE MIDI
    if(modoAcorde > nota)
    {
      BLEMidiServer.noteOn(Canal, rNota + tipoAcordes[modoAcorde][0], velocidad); // Primera nota del acorde
      BLEMidiServer.noteOn(Canal, rNota + tipoAcordes[modoAcorde][1], velocidad); // Segunda nota del acorde
    }
  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------
// Función para apagar (desactivar) una o más notas en el canal especificado, considerando acordes y conectividad BLE
void notasOff(uint8_t Canal, uint8_t rNota)
{
  // Si el dispositivo no está conectado por Bluetooth
  if(!bleConnected)
  {
    notaOff(Canal, rNota); // Apagar la nota en el canal especificado
    // Si el modo de acorde está activado, apaga las notas correspondientes al acorde seleccionado
    if(modoAcorde > nota)
    {
      notaOff(Canal, rNota + tipoAcordes[modoAcorde][0]); // Primera nota del acorde
      notaOff(Canal, rNota + tipoAcordes[modoAcorde][1]); // Segunda nota del acorde
    }
  }
  else  // Si el dispositivo está conectado por Bluetooth
  {
    BLEMidiServer.noteOff(Canal, rNota, 0); // Apagar la nota vía BLE MIDI
    // Apagar las notas del acorde a través de BLE MIDI
    if(modoAcorde > nota)
    {
      BLEMidiServer.noteOff(Canal, rNota + tipoAcordes[modoAcorde][0], 0); // Primera nota del acorde
      BLEMidiServer.noteOff(Canal, rNota + tipoAcordes[modoAcorde][1], 0); // Segunda nota del acorde
    }
  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------
// Función para gestionar la reproducción de notas, considerando digitación, soplido y cambios en el tono o el canal
void playNotes()
{
  // Determina la nota activa actual en función de la digitación de los sensores táctiles
  activePitch = noteMap[finalTouchStates];

  // Si el soplido está activo y la digitación corresponde al código de menú, activa el modo menú
  if(finalBreathState == HIGH && activePitch == 254)
  {
    activePitch = 0;
    modo = menu; // Cambia al modo de menú
  }

  // Si el soplido está activo y es la primera nota después de no tocar nada
  if(finalBreathState == HIGH && prevBreathState == LOW && activePitch != 0)
  {
    prevBreathState = HIGH; // Actualiza el estado del soplido
    notasOn(canal, (activePitch + transpose), velocity); // Activa la nota con la transposición y la velocidad actuales
    prevActivePitch = activePitch; // Guarda la nota activa para comparación futura
    prevCanal = canal; // Guarda el canal actual para comparación futura
  }

  // Si el soplido está activo y la digitación cambia a una nueva nota
  if(finalBreathState == HIGH && prevBreathState == HIGH && activePitch != prevActivePitch)
  {
    notasOff(canal, (prevActivePitch + transpose)); // Apaga la nota anterior
    if(activePitch != 0)
    {
      notasOn(canal, (activePitch + transpose), velocity); // Activa la nueva nota
    }
    prevCanal = canal; // Actualiza el canal para comparación
    prevActivePitch = activePitch; // Actualiza la nota activa para comparación
  }

  // Si el soplido está activo y la velocidad (intensidad del soplido) ha cambiado significativamente
  if(finalBreathState == HIGH && prevBreathState == HIGH && activePitch != 0 && abs(velocity - prevVelocity) > 4)
  {
    // Ajusta el volumen según el método de conexión
    if(bleConnected)
    {
      BLEMidiServer.controlChange(canal, 2, velocity); // Cambia la velocidad vía BLE MIDI
    }
    else
    {
      alterVolNota(canal, velocity); // Cambia el volumen de la nota
    }
    prevVelocity = velocity; // Actualiza la velocidad para comparación
  }

  // Si el soplido está activo y se cambia de canal
  if(finalBreathState == HIGH && prevBreathState == HIGH && canal != prevCanal)
  {
    notasOn(canal, (activePitch + transpose), velocity); // Activa la nota en el nuevo canal
    notasOff(canal, (prevActivePitch + transpose)); // Apaga la nota en el canal anterior
    prevCanal = canal; // Actualiza el canal para comparación
  }

  // Si el soplido se detiene, apaga la nota y, opcionalmente, muestra la nota o acorde en pantalla
  if(finalBreathState == LOW && prevBreathState == HIGH)
  {
    prevBreathState = LOW; // Bloquea la entrada hasta que se suelte
    notasOff(canal, (prevActivePitch + transpose)); // Apaga la nota

    // Muestra la nota o acorde en pantalla si "verNotas" está activo
    if(verNotas)
    {
      uint8_t vNota = prevActivePitch + transpose; // Nota transpuesta para mostrar
      if(modoAcorde == nota) // Muestra nota única
      {
        Menu.Message(String(Instrumentos[instrument + 1] + "|Nota|" + midiToNota(vNota)).c_str(), CENTER, 0);
      }
      else  // Muestra acorde
      {
        Menu.Message(String(Instrumentos[instrument + 1] + "| |Acorde|" +
                    midiToNota(vNota).c_str() + "," +
                    midiToNota(vNota + tipoAcordes[modoAcorde][0]).c_str() + "," +
                    midiToNota(vNota + tipoAcordes[modoAcorde][1]).c_str()), CENTER, 0);
      }
    }
  }
  Menu.UpdateOff(); // Actualiza el menú al finalizar la reproducción de notas
}
//---------------------------------------------------------------------------------------------------------------------------------------------
// Función para ajustar el valor de transposición de notas en semitonos
void transponer()
{
  bool up = false, down = false, salir = false; // Variables para detectar si se presiona subir, bajar o salir
  while(salir == false)
  {
    up = key7.isClicks(1);   // Detecta si se presiona el botón para aumentar
    down = key6.isClicks(1); // Detecta si se presiona el botón para disminuir
    salir = key8.isClicks(1); // Detecta si se presiona el botón para salir del ajuste de transposición

    // Ajusta el valor de transposición: reinicia a 0 si supera 5, o lo incrementa
    if(up) {  transpose = (transpose > 5) ? 0 : transpose += 1; }
    // Decrementa el valor de transposición: reinicia a 0 si es menor que -5
    else if(down) { transpose = (transpose < -5) ? 0 : transpose -= 1;  }

    // Muestra el valor de transposición en el menú (en semitonos), indicando si es positivo o negativo
    if(transpose > 0)  { Menu.Message("Transponer|+" + String(transpose) + "|Semitonos", CENTER, 10); }
    else if(transpose < 0) { Menu.Message("Transponer|" + String(transpose) + "|Semitonos", CENTER, 10); }
    else { Menu.Message("Transponer|" + String(transpose) + "|Semitonos", CENTER, 10); }
  }
  // Mensaje de confirmación y guardado de la configuración de transposición
  Menu.Message("Transponer||Establecido", CENTER, MSG_TIME);
  preferencias.putShort("Transpose", transpose); // Guarda el valor de transposición en memoria no volátil
}
//---------------------------------------------------------------------------------------------------------------------------------------------
// Función para ajustar el canal MIDI activo
void set_canal()
{
  bool up = false, down = false, salir = false; // Variables para detectar los botones de navegación en el menú
  while(salir == false)
  {
    up = key7.isClicks(1);   // Detecta si se presiona el botón para aumentar el canal
    down = key6.isClicks(1); // Detecta si se presiona el botón para disminuir el canal
    salir = key8.isClicks(1); // Detecta si se presiona el botón para salir del ajuste

    // Aumenta el canal hasta 2, luego reinicia a 0
    if(up) { canal = (canal > 1) ? 0 : canal += 1; }
    // Disminuye el canal hasta 0, luego reinicia a 2
    else if(down) { canal = (canal < 1) ? 2 : canal -= 1; }

    // Muestra el canal seleccionado en el menú
    Menu.Message("Canal||" + String(canal), CENTER, 10);
  }
  // Mensaje de confirmación y guardado de la configuración del canal
  Menu.Message("Canal||Establecido", CENTER, MSG_TIME);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
// Función para seleccionar el modo de acorde activo
void setModoAcorde()
{
  bool up = false, down = false, salir = false; // Variables de navegación para el menú de acordes
  txtMenu nomModos[] = {"Nota","Acord Mayor","Acord Menor","Acord Aume","Acord Dism"}; // Nombres de los modos de acorde
  while(salir == false)
  {
    up = key7.isClicks(1);   // Detecta si se presiona el botón para avanzar al siguiente acorde
    down = key6.isClicks(1); // Detecta si se presiona el botón para retroceder al acorde anterior
    salir = key8.isClicks(1); // Detecta si se presiona el botón para salir del ajuste

    // cambia entre los modos de acorde en ambas direcciones
    if(up) { modoAcorde = (modoAcorde > 2) ? -1 : modoAcorde += 1; }
    else if(down) { modoAcorde = (modoAcorde < 0) ? 3 : modoAcorde -= 1; }

    // Muestra el modo de acorde seleccionado en el menú
    Menu.Message("Modo Acorde||" + String(nomModos[modoAcorde + 1]), CENTER, 10);
  }

  // Configura el número máximo de notas activas según el modo seleccionado
  if(modoAcorde == nota) { setMaxNotas(1); } // Modo de nota única
  else { setMaxNotas(3); } // Modo de acorde

  preferencias.putShort("ModAcorde", modoAcorde); // Guarda la configuración en la memoria no volátil
  Menu.Message("Modo Acorde||Establecido", CENTER, MSG_TIME); // Mensaje de confirmación
}
//---------------------------------------------------------------------------------------------------------------------------------------------
// Función para ajustar el volumen del instrumento
void set_volumen()
{
  bool up = false, down = false, salir = false; // Variables de navegación en el menú
  uint8_t vol = preferencias.getShort("Volumen", 4); // Carga el volumen inicial desde las preferencias o valor predeterminado

  while(salir == false)
  {
    up = key7.isClicks(1);   // Detecta si se presiona el botón para aumentar el volumen
    down = key6.isClicks(1); // Detecta si se presiona el botón para disminuir el volumen
    salir = key8.isClicks(1); // Detecta si se presiona el botón para salir del ajuste

    // Cicla el volumen de 0 a 50, reiniciando al límite opuesto si excede
    if(up) { vol = (vol > 49) ? 0 : vol += 1; }
    else if(down) { vol = (vol < 1) ? 50 : vol -= 1; }

    // Muestra el volumen actual en el menú
    Menu.Message(String(vol), CENTER, 10);
  }

  setVolumen(vol); // Establece el volumen ajustado
  preferencias.putShort("Volumen", vol); // Guarda el volumen en memoria no volátil
  Menu.Message("Volumen||Establecido", CENTER, MSG_TIME); // Mensaje de confirmación
}
//---------------------------------------------------------------------------------------------------------------------------------------------
// Función para cargar las preferencias de usuario almacenadas en la memoria no volátil
void cargar_preferencias()
{
  uint8_t sonido, vol;

  // Carga el volumen almacenado en preferencias, o el valor predeterminado
  vol = preferencias.getShort("Volumen", 4);
  printLine("Volumen" + String(vol)); // Imprime el volumen actual en consola (para depuración)
  setVolumen(vol); // Establece el volumen cargado

  // Carga el instrumento seleccionado por el usuario o usa el predeterminado
  instrument = preferencias.getShort("Instrumento", 0);
  setInstrumento(0, instrument); // Configura el instrumento actual

  // Carga el valor de transposición
  transpose = preferencias.getShort("Transpose", 0);

  // Carga el modo de acorde almacenado y ajusta el número máximo de notas en consecuencia
  modoAcorde = preferencias.getShort("ModAcorde", nota);
  if(modoAcorde == nota) { setMaxNotas(1); }
  else { setMaxNotas(3); }

  // Carga el estado de efectos de sonido (reverb, chorus, tremolo)
  _Set.effectReverb = preferencias.getBool("FxReverb", false);
  _Set.effectChorus = preferencias.getBool("FxChorus", false);
  _Set.effectTremolo = preferencias.getBool("FxTremolo", false);

  // Carga la configuración de visualización de notas
  verNotas = preferencias.getBool("verNotas", true);

  // Carga la preferencia de salida de sonido (interno o BLE MIDI)
  sonido = preferencias.getShort("sonido", interno);
  if(sonido == blemidi) { BLEMidiServer.begin(NOMBRE); } // Activa BLE MIDI si es la configuración seleccionada
}
//---------------------------------------------------------------------------------------------------------------------------------------------
// Función para gestionar la navegación del menú principal y sus submenús
void funcMenu()
{
  bool up, down, enter;                   // Variables para detectar interacciones de navegación en el menú
  uint8_t sonido, bluetooth, pantalla, opcion; // Variables auxiliares

  up = key7.isClicks(1);                   // Detecta si se presiona el botón para subir en el menú
  down = key6.isClicks(1);                 // Detecta si se presiona el botón para bajar en el menú
  enter = key8.isClicks(1);                // Detecta si se presiona el botón para seleccionar una opción en el menú

  Menu.Show();                             // Muestra el menú en pantalla

  if(up){ Menu.Up(); }                     // Navega hacia arriba en el menú si "up" está activo
  if(down) { Menu.Down(); }                // Navega hacia abajo en el menú si "down" está activo

  // Menú principal - Modo "Tocar"
  else if(enter && Menu.Select(1, "Main")) // Si se selecciona la opción "Tocar" en el menú principal
  {
    // Mensaje de confirmación de entrada en el modo de tocar
    Menu.Message(String(NOMBRE) + "| |Play|Mode", CENTER, MSG_TIME);
    modo = tocar;                          // Cambia al modo tocar
  }

  // Submenú de Instrumentos
  else if(enter && Menu.Select(2, "Main")) // Si se selecciona la opción "Instrumentos" en el menú principal
  {
    set_canal();                           // Abre la configuración de canal
    Menu.Assign(Instrumentos, "Instr");    // Asigna el menú de instrumentos como submenú
    enter = false;                         // Reinicia la variable "enter"
  }

  // Navegación dentro del submenú "Instrumentos"
  for(uint8_t k = 1; k < totalInstr - 1; k++)  // Recorre todas las opciones de instrumentos
  {
    if(enter && Menu.Select(k, "Instr"))       // Si se selecciona un instrumento específico
    {
      // Muestra el nombre del instrumento seleccionado
      Menu.Message("Instrumentos| |" + Instrumentos[k] + "|Activo", CENTER, MSG_TIME);
      instrument = k - 1;                      // Guarda el índice del instrumento seleccionado
      enter = false;
      Menu.fullDown();                         // Actualiza la navegación del menú
      break;
    }
  }

  // Opción "Salir" en el submenú "Instrumentos"
  if(enter && Menu.Select(totalInstr - 1, "Instr"))
  {
    preferencias.putShort("Instrumento", instrument);   // Guarda el instrumento seleccionado en las preferencias
    setInstrumento(canal, instrument);                  // Configura el instrumento en el canal actual
    Menu.Root();                                        // Vuelve al menú principal
  }

  //---------------------------------------------------------------------------------------------------------------------------------------------
  // Submenú Volumen
  else if(enter && Menu.Select(3, "Main")) { Menu.Assign(Volumen, "Vol");  }
  else if(enter && Menu.Select(1, "Vol"))  { set_volumen();  Menu.fullDown();  }
  else if(enter && Menu.Select(2, "Vol"))  { Menu.Root(); }

  //---------------------------------------------------------------------------------------------------------------------------------------------
  // Submenú Efectos
  else if(enter && Menu.Select(4, "Main")) {  Menu.Assign(Efectos, "Efect"); }

  // Opción "Reverberación" en el submenú "Efectos"
  else if(enter && Menu.Select(1, "Efect"))
  {
    _Set.effectReverb = !_Set.effectReverb;                       // Cambia el estado del efecto de reverberación
    String estadoEfect = _Set.effectReverb ? "ON" : "OFF";        // Determina si el efecto está activado o desactivado
    Menu.Message("Efecto||Reververacion|" + estadoEfect, CENTER, MSG_TIME); // Muestra el estado del efecto
    preferencias.putBool("FxReverb", _Set.effectReverb);          // Guarda el estado en las preferencias
    Menu.fullDown();
  }

  // Opción "Chorus" en el submenú "Efectos"
  else if(enter && Menu.Select(2, "Efect"))
  {
    _Set.effectChorus = !_Set.effectChorus;                       // Cambia el estado del efecto de chorus
    String estadoEfect = _Set.effectChorus ? "ON" : "OFF";
    Menu.Message("Efecto||Chorus|" + estadoEfect, CENTER, MSG_TIME);
    preferencias.putBool("FxChorus", _Set.effectChorus);
    Menu.fullDown();
  }

  // Opción "Tremolo" en el submenú "Efectos"
  else if(enter && Menu.Select(3, "Efect"))
  {
    _Set.effectTremolo = !_Set.effectTremolo;                     // Cambia el estado del efecto de trémolo
    String estadoEfect = _Set.effectTremolo ? "ON" : "OFF";
    Menu.Message("Efecto||Tremolo|" + estadoEfect, CENTER, MSG_TIME);
    preferencias.putBool("FxTremolo", _Set.effectTremolo);
    Menu.fullDown();
  }
  else if(enter && Menu.Select(4, "Efect")) {   Menu.Root();  }    // Vuelve al menú principal

  //---------------------------------------------------------------------------------------------------------------------------------------------
  // Submenú Filtros
  else if(enter && Menu.Select(5, "Main")) {   Menu.Assign(Filtros, "Filtros");  }
  else if(enter && Menu.Select(3, "Filtros")) {    Menu.Root();  }

  //---------------------------------------------------------------------------------------------------------------------------------------------
  // Submenú Configuración
  else if(enter && Menu.Select(6, "Main"))  {   Menu.Assign(Config, "Conf");  }

  //---------------------------------------------------------------------------------------------------------------------------------------------
  // Opción "Sonido" en el submenú "Configuración"
  else if(enter && Menu.Select(1, "Conf")) {    Menu.Assign(Sonido, "Sonido");  }

  // Configuración de sonido interno
  else if(enter && Menu.Select(1, "Sonido"))
  {
    Menu.Message("Sonido|Parlante|interno|Activada", CENTER, MSG_TIME); // Muestra el estado del sonido interno
    BLEMidiServer.end();   // Finaliza la conexión BLE MIDI si está activa
    sonido = interno;      // Configura la salida de sonido como interna
  }

  // Configuración de sonido por BLE MIDI
  else if(enter && Menu.Select(2, "Sonido"))
  {
    Menu.Message("Sonido|Ble-Midi|Server|Activado", CENTER, MSG_TIME);   // Muestra el estado de conexión BLE MIDI
    Menu.Message("Bluetooth| |Esperando|Conexion", CENTER, MSG_TIME);
    BLEMidiServer.begin(NOMBRE);      // Inicia el servidor BLE MIDI
    sonido = blemidi;                 // Configura la salida de sonido como BLE MIDI
    Menu.fullDown();
  }
  else if(enter && Menu.Select(3, "Sonido")) { Menu.Root(); preferencias.putShort("sonido", sonido); }

  //---------------------------------------------------------------------------------------------------------------------------------------------
  // Opción "Transponer" en el submenú "Configuración"
  else if(enter && Menu.Select(2, "Conf")) {  transponer();  Menu.fullDown();  }

  //---------------------------------------------------------------------------------------------------------------------------------------------
  // Opción "Modo Acorde" en el submenú "Configuración"
  else if(enter && Menu.Select(3, "Conf")) {  setModoAcorde();  Menu.fullDown();  }

  //---------------------------------------------------------------------------------------------------------------------------------------------
  // Opción "Modo Sensor" en el submenú "Configuración"
  else if(enter && Menu.Select(5, "Conf")) {   Menu.Assign(ModoSensor, "ModoSens");  }
  else if(enter && Menu.Select(1, "ModoSens"))  {}
  else if(enter && Menu.Select(2, "ModoSens"))  {}
  else if(enter && Menu.Select(3, "ModoSens"))  {  Menu.Root(); }

  //---------------------------------------------------------------------------------------------------------------------------------------------
  // Opción "Ver Notas" en el submenú "Configuración"
  else if(enter && Menu.Select(6, "Conf"))
  {
    verNotas = !verNotas;                                       // Cambia el estado de visualización de notas
    String estVerNotas = verNotas ? "ON" : "OFF";               // Muestra si la visualización está activada o desactivada
    Menu.Message("Ver Notas||" + estVerNotas, CENTER, MSG_TIME);
    preferencias.putBool("verNotas", verNotas);                 // Guarda el estado en las preferencias
    Menu.fullDown();
  }

  //---------------------------------------------------------------------------------------------------------------------------------------------
  // Opción "Reset" en el submenú "Configuración"
  else if(enter && Menu.Select(7, "Conf")) {  Menu.Message("Reset| |Configuracion|Original", CENTER, MSG_TIME);  }

  //---------------------------------------------------------------------------------------------------------------------------------------------
  // Opción "Salir" en el submenú "Configuración"
  else if(enter && Menu.Select(8, "Conf")) {  Menu.Root();  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
void setup()
{
 // Serial.begin(115200); // Start serial communication
  BLEMidiServer.setOnConnectCallback([]()
  {
    bleConnected = true;
    printLine("Connected");
    Menu.Message("Bluetooth| |Estado|Conectado", CENTER, MSG_TIME);
  });
  BLEMidiServer.setOnDisconnectCallback([]()
  {
    bleConnected = false;
    printLine("Disconnected");
    Menu.Message("Bluetooth| |Estado|Desconectado", CENTER, MSG_TIME);
  });
  display.begin(); //initialize display
  Menu.Message(String(NOMBRE) + "|Iniciando|By: Holman|2024", CENTER, 1);
  Wire.begin();
  while(mpu.begin()) {}
  mpu.calcOffsets(true, true); // gyro and accelero
  preferencias.begin("Hewi", false);
  setNoteMap();
  initSynth(26, 25, 22);
  setMenuInstrumentos();
  //setDelayNota(10); 
  setFadeIn(10); 
  myBreath.setResistance(1); // Nivel de resistencia 3 (intermedio)
  myBreath.setMaxOut(127); // Nivel de resistencia 3 (intermedio)
  breathBaseVal = myBreath.read(); // Set neutral breath pressure level
  breathActiveThreshold = breathBaseVal + UMBRAL;
  Menu.SetAutoOff(30);
  Menu.alignTitle = CENTER;
  Menu.alignItem = LEFT;
  Menu.Root(Main, "Main");
  cargar_preferencias();
  modo = menu;
  Menu.Message(String(NOMBRE) + "|Listo|By: Holman|2024", CENTER, MSG_TIME);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop()
{
  if(modo == menu)  {  funcMenu(); }
  else
  {
    readBreath();
    //programTime = Millis();
    //if((programTime - prevProgramTime) > 2)
    {
      readKeys();
      if(activeBend){mpu.update(); //if(mpu.getAccX()*100<50){canal=1;}else{canal=0;}}
      float tiltX = mpu.getAccX()*-100;
      int16_t  pitchOffset =(tiltX>-45)?map(tiltX, -45, 10, 1, 100):0; // Ajusta los valores según el rango que prefieras
      alterPitchNota(canal, pitchOffset);
    //  Serial.print("Inclinación X: "); Serial.print(tiltX); Serial.print(" | Pitch Offset: "); Serial.println(pitchOffset);
      }
      playNotes();
      //prevProgramTime = programTime;
    }
  }
}
