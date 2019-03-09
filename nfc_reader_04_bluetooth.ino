/* NFC reader with color display and bluetooth link (serial echo) to android phone 
 * selfedu project for arduino uno/nano, under GNU/GPL
 * 
 * version 0.4, rare demo, 03/2019.
 * 
 * Used generic devices: 
 * Arduino NANO/UNO, TFT display 1.44" (ST7735, SPI), NFC modul MFRC522(SPI), Bluetooth modul HC06(UART).
 * 
 * Proof of concept:
 * two SPI devices
 * quick reading UID of all Mifare/NFC devices(TAG), 
 * send UID to TFT, 
 * show version of transpoder, and send it via bluetooth to terminal on android phone. 
 * 
 * Used terminal software -tested (available on google play repository): 
 * Serial Bluetooth Terminal v.1.22 by Kai-Morich
 * 
 * Library and parts of code are from GitHub, Arduino-Shop.cz and Arduino.cc
 *
 */
 
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <MFRC522.h>

#define TFT_CS        10  // CS for TFT (SlaveSelect SPI bus TFT)
#define TFT_RST        9  // Reset for TFT
#define TFT_DC         8  // DC(A0) for TFT
#define TFT_BACKLIGHT  6  // TFT backlight pin
#define RX             2  // serial data RX modul HC04/06
#define TX             3  // serial data TX modul HC04/06
#define RC522_SS       5  // slave select mfrc522 modul
#define RC522_RST      4  // reset mfrc522 modul         
const int piskle =     7;  // beep feedback (if UID was readed)

// used default HARDWARE SPI pins Uno/Nano: 
// SPI MOSI = pin 11
// SPI MISO = pin 12
// SPI SCLK = pin 13 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST); // instance tft s radicem ST7735
SoftwareSerial bluetooth(TX, RX);                               // instance modrozubu(default pair PIN 1234)
MFRC522 mfrc522(RC522_SS, RC522_RST);                           // instance nfc modulu MFRC522

void pipni()  {               //rutina pipnuti 50ms
  digitalWrite(piskle, HIGH);
  delay(50);
  digitalWrite(piskle, LOW);
  }

void setup()  {
  Serial.begin(9600);       // serial init
  SPI.begin();              // SPI bus init
  mfrc522.PCD_Init();       // init MFRC522
  bluetooth.begin(9600);    // bluetooth init
  
  pinMode(piskle, OUTPUT); 
    
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight TFT on
  
  tft.initR(INITR_BLACKTAB);        // init 160x128 verze tft
  //tft.initR(INITR_GREENTAB);      // init 128x128 verze tft
  tft.setRotation(3);               // otoc displej na sirku s kontaktama vlevo
  welcomeScreen();                  // ukaz welcome text
  pipni();                          // pipni
}  
  
void printHex(byte *buffer, byte bufferSize)    //rutina pro vypis bufferu (uid) v HEX
  {
  for (byte i = 0; i < bufferSize; i++) 
    {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    }
  }

/*
void printDec(byte *buffer, byte bufferSize)    //rutina pro vypis bufferu (uid) v DEC
  {
  for (byte i = 0; i < bufferSize; i++) 
    {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
    }
  }
*/

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
  return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  clearTFT();
  pipni();
  readRFID();
 }

void welcomeScreen(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(3,30);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_RED);
  tft.print("NFC ctecka"); 
  bluetooth.println("");
  bluetooth.println("NFC ctecka");
  tft.setCursor(3,70);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.print("Radarixos, ver. 0.4");bluetooth.println("Radarixos, ver. 0.4");
  tft.setCursor(3,90);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print("Bluetooth serial"); bluetooth.println("Bluetooth - serial link seems OK!");
}

void clearTFT() {
  tft.fillScreen(ST77XX_BLACK);     // cerne pozadi
}

void readRFID() {
// new record
  Serial.print("\n-----------------------------");
  bluetooth.print("\n-----------------------------:");
  
//picc version
  Serial.print("\nVerze: ");
  bluetooth.println("\nVerze:");
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
    bluetooth.print(mfrc522.PICC_GetTypeName(piccType));
    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(0,10);
    tft.setTextSize(1);
    tft.print("Verze transpoderu:");
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(0,20);
    tft.setTextSize(1);
    tft.print(mfrc522.PICC_GetTypeName(piccType));

//picc UID
    Serial.print("UID v hex: ");
    String content= "";           //vynulovani promenne "content"
    bluetooth.println("\nUID v hex:");
    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(0,40);
    tft.setTextSize(1);
    tft.print("UID v hex: ");
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(0,50);
        
  for (byte i = 0; i < mfrc522.uid.size; i++) //cti a posilej uid dal
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
     //tft.print(String(mfrc522.uid.uidByte[i], HEX));            //zapis bez mezer mezi bytama
     tft.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");       //zapis s mezerama mezi bytama
     tft.print(mfrc522.uid.uidByte[i], HEX);                      //zapis s mezerama mezi bytama
     bluetooth.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "); //zapis s mezerama mezi bytama
     bluetooth.print(mfrc522.uid.uidByte[i], HEX);                //zapis s mezerama mezi bytama
  }


  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

}
