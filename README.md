#### **[[ENGLISH]](#spanish)**

# HEWI - Electronic Wind Instrument

HEWI (Holman's Electronic Wind Instrument) Es un instrumento de viento electrónico, creado para makers, músicos y entuciastas que buscan explorar su potencial creativo, modificando, programando o personalizando tu propio EWI. Este instrumento esta basado en la placa lolin32 lite y para el he implementado varias librerias personalizadas puedes encontrar aqui. 
Mas informacion en <a href=https://sites.google.com/view/hewi-instrument>HEWI</a>.
<p align="center">
<img width="15%" src="https://i.ibb.co/0qbKgFJ/media-1.png" border="0">
</p>

## Características

- **Control de Soplo:** Gracias a la biblioteca Breath se ha optimizado la lectura de muestras para ser usadas como sensor de soplo permitiendo configurar la resistencia y establecer valores de umbral y lectura maxima.
- **Botones Capacitivos:** La libreria TouchButton gestiona eficazmente los pines capacitivos que se usan en la digitacion del instrumento.
- **Motor de Síntesis:** Utiliza samplerSynth para la generación de sonido.
- **Interfaz de Menú:** Control fácil de las configuraciones del instrumento con la biblioteca simpleMenu.
- **Conexion BLE-MIDI:** Conectividad Bluetooth MIDI para integrar con dispositivos externos

## Requisitos
### Hardware (Partes y modulos):
Estas son las partes y modulos que debes conseguir si quieres armar tu mismo el proyecto HEWI. Proximamente estara disponible el kit que viene con todo lo neceario para enamblarlo y con un acabado mas profesional

| Parte | Imagen |
|--------------|---|
| ESP32 (Modelo Wemos Lolin32 lite) |<p align="center"> <img  width="50%" src="https://i.ibb.co/n3dQx84/lolin32.jpg" alt="lolin32" border="0"> </p>|
| Sensor de presion MPS20N0040D-HX710B | <p align="center"> <img width="40%" src="https://i.ibb.co/2jp6Q5j/hx710b.jpg" alt="hx710b" border="0"></a></p>|
| Pantalla OLED 128x64 I2C |<p align="center"> <img  width="40%" src="https://i.ibb.co/4RZXHvp/ssd1306.jpg" alt="ssd1306" border="0"> </p> |
| DAC MAX98357A (Actualmente) | <p align="center"> <img width="40%" src="https://i.ibb.co/kMWgXYL/max98357a.jpg" alt="max98357a" border="0"> </p> |
| Conector Audifonos | <p align="center"> <img width="40%" src="https://i.ibb.co/C7YdgVt/jack3-5.jpg" alt="jack3-5" border="0"> </p> |
| Altavoz 3W 4ohm | <p align="center"> <img width="40%" src="https://i.ibb.co/x3Ct0g7/speaker3w.jpg" alt="speaker3w" border="0"></p> |
| Sensor MPU-6050 (gy-521) | <p align="center"> <img width="40%" src="https://i.ibb.co/fXpvR2H/mpu6050.jpg" alt="mpu6050" border="0"></p>  |
| Bateria litio 3.7v |  <p align="center"> <img width="40%" src="https://i.ibb.co/0K75nGB/bateria3-7.jpg" alt="bateria3-7" border="0"></p>  |
| 40cm tubo silicona 3mm-4mm|  <p align="center"> <img width="40%" src="https://i.ibb.co/dDjdCkg/tubosilicona.jpg" alt="tubosilicona" border="0"></p> |
| Chinchetas o tachuelas |  <p align="center"> <img width="40%" src="https://i.ibb.co/VDx5L7B/tachuelas.jpg" alt="tachuelas" border="0"></p> |


## Software y librerias:
- IDE de Arduino: Recomendado 1.8.10 o superior
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

- Conecta el sensor de soplo MPS20N0040D a los pines del ESP32 (data out=5, clock=18).
- Conecta el sensor de movimiento MPU-6050  en los pines de comunicación I2C de la placa (sda=19,scl=23).
- Conecta la pantalla OLED I2C en los pines de comunicación I2C de la placa (sda=19,scl=23).
- Conecta el modulo DAC  MAX98357  a los pines  de comunicación I2S Seleccionados .
- Conecta la salida de audifonos y el parlante al modulo DAC MAX98357.
- Conecta las pines capacitivos a los contactos metalicos.

## Configuración y Uso

### Opciones del Menu

- Tocar: El modo de interpretación activa los sensores de soplido y los botones de digitación para generar notas.
- Instrumentos: Cambia el sonido utilizando el menú de instrumentos.
- Volumen: Ajusta el volumen del instrumento.
- Efectos y Filtros: Aplica efectos como reverberación, chorus, y filtro paso bajo.
- Transposición: Ajusta la afinación del instrumento en semitonos.
- Modo Acorde: Cambia el modo de acordes según la selección.

# Comunidad
¡Las contribuciones son bienvenidas! Si tienes sugerencias, encuentra un error o deseas agregar una nueva característica, por favor abre un issue o envía un pull request.

# Donaciones
Si te gusto este proyecto o simplemente te sientes generoso, considera invitarme una cerveza. ¡Salud! 🍻
¡Tu apoyo es invaluable!
