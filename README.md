#### **[[ENGLISH]](#spanish)**

# HEWI - Electronic Wind Instrument
HEWI (Holman's Electronic Wind Instrument) Es un instrumento de viento electrónico, creado para makers, músicos y entuciastas que buscan explorar su potencial creativo, modificando, programando o personalizando tu propio EWI. Este instrumento esta basado en la placa lolin32 lite y para el he implementado varias librerias personalizadas puedes encontrar aqui. 
Mas informacion en <a href=https://sites.google.com/view/hewi-instrument>HEWI</a>.
<p align="center">
<img width="15%" src="https://i.ibb.co/K9cwmTc/media.png" alt="media" border="0">
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
| Bateria litio 3.7v 400mAh o mas |  <p align="center"> <img width="40%" src="https://i.ibb.co/0K75nGB/bateria3-7.jpg" alt="bateria3-7" border="0"></p>  |
| 40cm tubo silicona 3mm-4mm|  <p align="center"> <img width="40%" src="https://i.ibb.co/dDjdCkg/tubosilicona.jpg" alt="tubosilicona" border="0"></p> |
| Chinchetas o tachuelas |  <p align="center"> <img width="40%" src="https://i.ibb.co/VDx5L7B/tachuelas.jpg" alt="tachuelas" border="0"></p> |
| Baquelita perforada o universal |  <p align="center"> <img width="40%" src="https://i.ibb.co/6JRVJdn/baquelita.jpg" alt="baquelita" border="0"></p> |
 

## Software y librerias:
- IDE de Arduino: Recomendado 1.8.10 o superior

### Librerias propias
  - <a href="https://github.com/habuenav/samplerSynth">samplerSynth</a>: Síntesis de sonido, gestion de las notas y efectos.
  - <a href="https://github.com/habuenav/Breath">Breath</a>: Gestionar la entrada del sensor de soplo
  - <a href="https://github.com/habuenav/TouchButton">TouchButton</a>: Gestion de los sensores capacitivos.
  - <a href="https://github.com/habuenav/simpleMenu">simpleMenu</a>: Creacion y gestion de menús de configuración.
 
### Librerias de terceros
  - <a href="https://github.com/max22-/ESP32-BLE-MIDI">ESP32-BLE-MIDI</a>: Librería para usar Midi sobre Bluetooth Low Energy (BLE) en placas ESP32.
  - Preferences :  Libreria para guardar datos en la memoria flash incluida con la instalacion de soporte ESP32 en el ide Arduino
  - <a href="https://github.com/rfetick/MPU6050_light">Mpu-6050-light</a>: Libreria diseñada para obtener mediciones del acelerómetro y del giroscopio del MPU6050 (gy-521)

## Montaje


- Carcasa: En desarrollo (No disponible aun) Imprime la carcasa del instrumento usando los archivos STL incluidos (o puedes diseñar una propia), tambien puedes ser creativo y usar una seccion de canaleta de almenos estas dimenciones (40mm Ancho x 25mm Alto x 250mm Largo)
- Boquilla: En desarrollo (No disponible aun) Imprime la boquilla del instrumento usando los archivos STL incluidos (o puedes diseñar una propia), o puedes usar una boquilla recuperada de un Vapper y adaptarla tal como hice con mi primer prototipo.<br>
<img width="12%" src="https://i.ibb.co/g92fLJ6/canaleta.jpg" alt="canaleta" border="0"> <img width="12%" src="https://i.ibb.co/10pQjtg/boquillavaper.jpg" alt="boquillavaper" border="0"></a> el tubo o manguera de silicona va de la boquilla al sensor de soplo y otra parte va de la boquilla al extremo inferior del instrumento como salida de aire y saliva.

### Conexión de Componentes:
Para la conexion de componentes puedes guiarte del siguiente esquema

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
