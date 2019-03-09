# nfc_reader_mfrc522_tft_st7735_demo

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
 *pinouts arduino UNO:
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

MFRC522 need 3V supply.
TFT disp. working with 5V.
Bluetooth modul need 5V.
 */
