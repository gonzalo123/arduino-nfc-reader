/*
  Turns on an LED on for one second, then off for one second, repeatedly.

  Read NFC Tag and send tag ID to a remote TCP server
  If ID is a valid ID server returns OK and digital pin is set to 1 (to open a door for example)

  Gonzalo Ayuso
  
  MIT License
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>
#include <EthernetClient.h>

#define RST_PIN 9
#define SS_PIN  8
#define ERROR_PIN 7
#define OPEN_PIN 6
#define OPEN_DELAY 2000

char server[] = "192.168.1.104";
int port = 28001;

signed long timeout;

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
MFRC522 mfrc522(SS_PIN, RST_PIN);
EthernetClient client;

void printArray(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

String dump_byte_array(byte *buffer, byte bufferSize) {
          String out = "";
    for (byte i = 0; i < bufferSize; i++) {
        out += String(buffer[i] < 0x10 ? " 0" : " ") + String(buffer[i], HEX);
    }
    out.toUpperCase();
    out.replace(" ", "");
    
    return out;
}

void resetLeds() {
  digitalWrite(OPEN_PIN, LOW);
  digitalWrite(ERROR_PIN, LOW);
}

void open() {
  Serial.println("OPEN!");
  digitalWrite(OPEN_PIN, HIGH);
  delay(OPEN_DELAY);
  digitalWrite(OPEN_PIN, LOW);
}

void error() {
  Serial.println("ERROR!");
  digitalWrite(ERROR_PIN, HIGH);
  delay(OPEN_DELAY);
  digitalWrite(ERROR_PIN, LOW);
}

void scanCard() {
  byte status;
  byte buffer[18];
  int err = 0;
  byte size = sizeof(buffer);
  EthernetClient c;
      
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      const String ID = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
      Serial.println("New tag read: " + ID);
      mfrc522.PICC_HaltA();
     
      if (client.connect(server, port)) {
        timeout = millis() + 3000;
        client.println("OPEN:" + ID);
        delay(10);

        while(client.available() == 0) {
          if (timeout - millis() < 1000) {
              error();
              goto close;
          }
        } 
        int size;
        bool status;
        
        while((size = client.available()) > 0) {
          uint8_t* msg = (uint8_t*)malloc(size);
          size = client.read(msg,size);
          //Serial.write(msg, size);
          // 4F4B   -> OK
          // 4E4F4B -> NOK
          status = dump_byte_array(msg, size) == "4F4B";
          free(msg);
        }
        
        Serial.println(status ? "OK!" : "NOK!");
        if (status) {
          open();
        } else {
          error();
        }
close:
        client.stop();
      } else {
        Serial.println("Connection Error");
        error();
      }
    }
  }
}

void setup()
{
  resetLeds();
  Serial.begin(9600);
  Serial.println("Setup ...");

  // disable w5100 SPI
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  SPI.begin();
  mfrc522.PCD_Init();

  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP Error");
    error();
    while (true) {}
  }
  Serial.print("My IP: ");
  for (byte B = 0; B < 4; B++) {
    Serial.print(Ethernet.localIP()[B], DEC);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Finish setup");
  timeout = 0;
}

void loop()
{
  resetLeds();
  scanCard();
  delay(200);
}
