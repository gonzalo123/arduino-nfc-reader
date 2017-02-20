Arduino NFC tag reader
======

# Project
* Read NFC tag
* validate the id of the tag against remote server
* if tag id is a valid one then trigger a digital output. If none then trigger another digital output

# MFRC522 Connection
* sda: 10 (*) -> 8
* sck: 13
* Mosi: 11
* Miso: 12
* Rq: --
* Gnd: Gnd
* Rst: 9
* 3.3V: 3.3V

(*) Take care:
If we use ethernet shield with a MFRC522 there's a SPI conflict (due to ethernet shield's SD card reader).
We need to use another sda pin (here I'm using pin 8 instead of 10) and disable w5100 SPI before configure ethernet
```
// disable w5100 SPI
pinMode(10, OUTPUT);
digitalWrite(10, HIGH);
```

# Hardware:
* Arduino UNO (https://www.arduino.cc/en/Main/ArduinoBoardUno)
* Ethernet shield W5100 (https://www.sparkfun.com/datasheets/DevTools/Arduino/W5100_Datasheet_v1_1_6.pdf)
* RFID/NFC Sensor MFRC522 (http://playground.arduino.cc/Learning/MFRC522)

# Server
* A simple TCP server with node
* To start the server
```
node node/server.js
```

# Demo
[![Validate NFC tags against node TCP server with arduino ](http://img.youtube.com/vi/hV4BeSx0Kw4/0.jpg)](https://www.youtube.com/watch?v=hV4BeSx0Kw4)
# References:
https://www.luisllamas.es/arduino-rfid-mifare-rc522/
http://hetpro-store.com/TUTORIALES/modulo-lector-rfid-rc522-rf-con-arduino/
https://forum.arduino.cc/index.php?topic=198768.0