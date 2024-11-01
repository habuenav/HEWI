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
Para la conexion de componentes puedes guiarte del siguiente esquema, la baquelita perforada esta como referencia para ubicar los componentes en ella, la conexion de los cables queda a tu gusto, se que los ubicaras de la mejor manera.
<p align="center"> <img src="https://i.ibb.co/CQgwBSx/esquema-HEWI.png" alt="esquema-HEWI" border="0"> </p>

## Configuración y Uso


### Opciones del Menu
- Tocar: El modo de interpretación activa los sensores de soplido y los botones de digitación para generar notas.
- Instrumentos: Cambia el sonido utilizando el menú de instrumentos.
- Volumen: Ajusta el volumen del instrumento.
- Efectos y Filtros: Aplica efectos como reverberación, chorus, y filtro paso bajo.
- Transposición: Ajusta la afinación del instrumento en semitonos.
- Modo Acorde: Cambia el modo de acordes según la selección.

## Comunidad
¡Las contribuciones son bienvenidas! Si tienes sugerencias, encuentra un error o deseas agregar una nueva característica, por favor abre un issue o envía un pull request.
## Donaciones
Si te gusto este proyecto o simplemente te sientes generoso, considera invitarme una cerveza. ¡Salud! :beers:<br/>
<a href="https://www.paypal.com/donate/?business=T8UBSMVJ2QT9Y&no_recurring=0&item_name=%C2%A1Gracias+por+tu+apoyo%21%0ATu+donaci%C3%B3n+es+de+gran+ayuda+y+es+un+incentivo+para+seguir+mejorando.&currency_code=USD"><img src="https://www.paypalobjects.com/digitalassets/c/website/marketing/latam/mx/accept-payments-online/icons/img_btn-donate2x.png" height="80"></a><br/>
Si has realizado una donación, por favor escribeme un correo y te enviare una version ligeramente optimizada y totalmente comentada, ademas de recibir rápidamente cualquier actualización o mejora adicional que haga a la librería. ¡Tu apoyo es invaluable!

#### **[[SPANISH]](#english)** 

# HEWI - Electronic Wind Instrument
HEWI (Holman's Electronic Wind Instrument) It is an electronic wind instrument, created for makers, musicians and enthusiasts who are looking to explore their creative potential, modifying, programming or customizing their own EWI. This instrument is based on the lolin32 lite board and for it I have implemented several custom libraries you can find here.
More information at<a href=https://sites.google.com/view/hewi-instrument>HEWI</a>.
<p align="center">
<img width="15%" src="https://i.ibb.co/K9cwmTc/media.png" alt="media" border="0">
</p>

## Features
- **Breath Control:** The Breath library has optimized sample reading for use as a breath sensor, allowing resistance to be configured and threshold and maximum reading values ​​to be set.
- **Capacitive Buttons:** The TouchButton library efficiently manages the capacitive pins used in the instrument's fingering.
- **Synthesis Engine:** Uses samplerSynth for sound generation.
- **Menu Interface:** Easy control of instrument settings with the simpleMenu library.
- **BLE-MIDI Connection:** Bluetooth MIDI connectivity for integration with external devices

## Requirements
### Hardware (Parts and modules):
These are the parts and modules that you must get if you want to build the HEWI project yourself. The kit that comes with everything necessary to assemble it and with a more professional finish will be available soon.
| Part | Image |
|--------------|---|
| ESP32 (Wemos Lolin32 lite Model) |<p align="center"> <img width="50%" src="https://i.ibb.co/n3dQx84/lolin32.jpg" alt="lolin32" border= "0"> </p>|
| Pressure sensor MPS20N0040D-HX710B | <p align="center"> <img width="40%" src="https://i.ibb.co/2jp6Q5j/hx710b.jpg" alt="hx710b" border="0"></a> </p>|
| 128x64 I2C OLED display |<p align="center"> <img width="40%" src="https://i.ibb.co/4RZXHvp/ssd1306.jpg" alt="ssd1306" border="0" > </p> |
| DAC MAX98357A (Currently) | <p align="center"> <img width="40%" src="https://i.ibb.co/kMWgXYL/max98357a.jpg" alt="max98357a" border="0"> </p> |
| Headphone Connector | <p align="center"> <img width="40%" src="https://i.ibb.co/C7YdgVt/jack3-5.jpg" alt="jack3-5" border="0"> </p> |
| Speaker 3W 4ohm | <p align="center"> <img width="40%" src="https://i.ibb.co/x3Ct0g7/speaker3w.jpg" alt="speaker3w" border="0"></p> |
| Sensor MPU-6050 (gy-521) | <p align="center"> <img width="40%" src="https://i.ibb.co/fXpvR2H/mpu6050.jpg" alt="mpu6050" border="0"></p> |
| Lithium battery 3.7v 400mAh or more | <p align="center"> <img width="40%" src="https://i.ibb.co/0K75nGB/bateria3-7.jpg" alt="bateria3-7" border="0"> </p> |
| 40cm silicone tube 3mm-4mm| <p align="center"> <img width="40%" src="https://i.ibb.co/dDjdCkg/tubosilicona.jpg" alt="tubosilicona" border="0"></p> |
| Thumbtacks or thumbtacks | <p align="center"> <img width="40%" src="https://i.ibb.co/VDx5L7B/tachuelas.jpg" alt="tachuelas" border="0"></p> |
| Perforated or universal bakelite | <p align="center"> <img width="40%" src="https://i.ibb.co/6JRVJdn/baquelita.jpg" alt="baquelita" border="0"></p> |
 

## Software and libraries:
- Arduino IDE: Recommended 1.8.10 or higher

### Own libraries 
- <a href="https://github.com/habuenav/samplerSynth">samplerSynth</a>: Sound synthesis, note management and effects.
- <a href="https://github.com/habuenav/Breath">Breath</a>: Manage breath sensor input.
- <a href="https://github.com/habuenav/TouchButton" >TouchButton</a>: Management of capacitive sensors.
- <a href="https://github.com/habuenav/simpleMenu">simpleMenu</a>: Creation and management of configuration menus.
 
### Third-party libraries
- <a href="https://github.com/max22-/ESP32-BLE-MIDI">ESP32-BLE-MIDI</a>: Library to use MIDI over Bluetooth Low Energy (BLE) on ESP32 boards.
- Preferences : Library to save data in the flash memory included with the ESP32 support installation in the Arduino IDE
- <a href="https://github.com/rfetick/MPU6050_light">Mpu-6050-light</a>: Library designed to obtain measurements from the accelerometer and gyroscope of the MPU6050 (gy-521)

## Assembly

- Housing: In development (Not available yet) Print the instrument housing using the included STL files (or you can design your own), you can also be creative and use a gutter section of at least these dimensions (40mm Width x 25mm Height x 250mm Length)
- Mouthpiece: In development (Not available yet) Print the instrument mouthpiece using the included STL files (or you can design your own), or you can use a mouthpiece recovered from a Vapper and adapt it as I did with my first prototype.<br>
<img width="12%" src="https://i.ibb.co/g92fLJ6/canaleta.jpg" alt="canaleta" border="0"> <img width="12%" src="https://i.ibb.co/10pQjtg/boquillavaper.jpg" alt="boquillavaper" border="0"></a> the silicone tube or hose goes from the mouthpiece to the puff sensor and another part goes from the mouthpiece to the lower end of the instrument as an air and saliva outlet.

### Connecting Components:
For the connection of components you can use the following diagram as a guide. The perforated bakelite is used as a reference to locate the components on it. The connection of the cables is up to you, I know that you will locate them in the best way.
<p align="center"> <img src="https://i.ibb.co/CQgwBSx/esquema-HEWI.png" alt="esquema-HEWI" border="0"> </p>

## Configuration and Use


### Menu Options
- Play: Performance mode activates the breath sensors and finger buttons to generate notes.
- Instruments: Change the sound using the instrument menu.
- Volume: Adjust the volume of the instrument.
- Effects & Filters: Apply effects such as reverb, chorus, and low-pass filter.
- Transpose: Adjust the pitch of the instrument in semitones.
- Chord Mode: Change the chord mode based on the selection.

## Community
Contributions are welcome! If you have suggestions, find a bug, or want to add a new feature, please open an issue or submit a pull request.
## Donations
If you liked this project or are just feeling generous, consider buying me a beer. Health! :beers:<br/>
<a href="https://www.paypal.com/donate/?business=T8UBSMVJ2QT9Y&no_recurring=0&item_name=%C2%A1Gracias+por+tu+apoyo%21%0ATu+donaci%C3%B3n+es+de+gran+ayuda+y+es+un+incentivo+para+seguir+mejorando.&currency_code=USD"><img src="https://www.paypalobjects.com/digitalassets/c/website/marketing/latam/mx/accept-payments-online/icons/img_btn-donate2x.png" height="80"></a><br/>
If you have made a donation, please write me an email and I will send you a slightly optimized and fully commented version, in addition to quickly receiving any additional updates or improvements I make to the library. Your support is invaluable!
