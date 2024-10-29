# HEWI - Electronic Wind Instrument

HEWI (Holman's Electronic Wind Instrument) Es un instrumento de viento electrónico, diseñado para Makers y músicos y entuciastas que buscan explorar su potencial creativo, modificando, programando o personalizando un su propio EWI. Este proyecto esta basado en el microcontrolador ESP32 y para el se han implementado varias bibliotecas personalizadas puedes encontrar mas informacion en su pagina oficial <a href=https://sites.google.com/view/hewi-instrument>HEWI</a>.

# Características

**Control de Soplo:** Gracias a la biblioteca Breath se ha optimizado la lectura de muestras para ser usadas como sensor de soplo permitiendo configurar la resistencia y establecer valores de umbral y lectura maxima.
**Teclas Capacitivas:** La libreria TouchButton gestiona eficazmente los pines capacitivos que se usan en la digitacion del instrumento.
**Motor de Síntesis:** Utiliza samplerSynth para la generación de sonido.
**Interfaz de Menú:** Control fácil de las configuraciones del instrumento con la biblioteca simpleMenu.
**Conexion BLE-MIDI:** Conectividad Bluetooth MIDI para integrar con dispositivos externos


# Requisitos

## Hardware (Partes y modulos):
- ESP32 (Modelo Wemos Lolin32 lite)
- Sensor de presion MPS20N0040D-HX710B
- Pantalla OLED 128x64 I2C
- DAC MAX98357A (Actualmente)
- Conector Audifonos
- Altavoz 3W 4ohm
- Sensor MPU-6050 (gy-521)
- Bateria litio 3.7v
- 

# Software y librerias:
- IDE de Arduino: Recomendado 1.8.10 o superior
- 
  ### Librerias personalizadas
  - samplerSynth: Biblioteca de síntesis de sonido utilizada para generar las notas.
  - Breath: Biblioteca para gestionar la entrada del sensor de soplo
  - TouchButton: Biblioteca para manejar los sensores capacitivos de digitación.
  - simpleMenu: Biblioteca para manejar los menús de configuración.
 
  ### librerias de terceros
  - Ble-Midi
  - Preferences
  - Mpu-6050-light

## Montaje

- Carcasa: En desarrollo(No disponible aun) Imprime la carcasa del instrumento usando los archivos STL incluidos (o puedes diseñar una propia).
- Conexión de Componentes:

Conecta el sensor de soplo MPS20N0040D a los pines del ESP32 (data out=5, clock=18).
Conecta el sensor de movimiento MPU-6050  en los pines de comunicación I2C de la placa (sda=19,scl=23).
Conecta la pantalla OLED I2C en los pines de comunicación I2C de la placa (sda=19,scl=23).
Conecta el modulo DAC  MAX98357  a los pines  de comunicación I2S Seleccionados .
Conecta la salida de audifonos y el parlante al modulo DAC MAX98357.
Conecta las pines capacitivos () a los contactos metalicos.

# Configuración y Uso

## Opciones del Menu

Modo Tocar: El modo de interpretación activa los sensores de soplido y los botones de digitación para generar notas.
Configuración del Menú:

Instrumentos: Cambia el sonido utilizando el menú de instrumentos.

Volumen: Ajusta el volumen del instrumento.

Efectos y Filtros: Aplica efectos como reverberación, chorus, y filtro paso bajo.

Transposición: Ajusta la afinación del instrumento en semitonos.

Modo Acorde: Cambia el modo de acordes según la selección.


# Comunidad
¡Las contribuciones son bienvenidas! Si tienes sugerencias, encuentra un error o deseas agregar una nueva característica, por favor abre un issue o envía un pull request.

# Donaciones
Si te gusto este proyecto o simplemente te sientes generoso, considera invitarme una cerveza. ¡Salud! 🍻
¡Tu apoyo es invaluable!
