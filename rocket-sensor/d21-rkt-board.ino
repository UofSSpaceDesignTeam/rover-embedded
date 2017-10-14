/***************************************************************************
 This is a library for the BMP280 humidity, temperature & pressure sensor

 Designed specifically to work with the Adafruit BMEP280 Breakout
 ----> http://www.adafruit.com/products/2651

 These sensors use I2C or SPI to communicate, 2 or 4 pins are required
 to interface.

 Adafruit invests time and resources providing this open source code,
 please support Adafruit andopen-source hardware by purchasing products
 from Adafruit!

 Written by Limor Fried & Kevin Townsend for Adafruit Industries.  
 BSD license, all text above must be included in any redistribution
***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

void setup() {
 SerialUSB.begin(9600);
 while (!SerialUSB);
 SerialUSB.println(F("BMP280 test"));
 
 if (!bmp.begin(0x76,0x58)) {  
   SerialUSB.println(F("Could not find a valid BMP280 sensor, check wiring!"));
   while (1);
 }
}

void loop() {
   SerialUSB.print(F("Temperature = "));
   SerialUSB.print(bmp.readTemperature());
   SerialUSB.println(" *C");
   
   SerialUSB.print(F("Pressure = "));
   SerialUSB.print(bmp.readPressure());
   SerialUSB.println(" Pa");

   SerialUSB.print(F("Approx altitude = "));
   SerialUSB.print(bmp.readAltitude(1015.92)); // this should be adjusted to your local forcase
   SerialUSB.println(" m");
   
   SerialUSB.println();
   delay(2000);
}
