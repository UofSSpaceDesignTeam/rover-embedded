#include "VESCPacket.h"
#include <string.h>
byte* inByte;
byte id = 100;
char* outByte;
uint8_t* payload;
int lenPay = 0;
int cnt = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  inByte = (byte*)malloc(sizeof(byte) * 256);
  outByte = (char*)malloc(sizeof(char) * 256);
  payload = (uint8_t*)malloc(sizeof(uint8_t) * 256);
  Serial.begin(115200);
  while(!Serial);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
      ReadVESCMessage(inByte);
      UnpackMessage(inByte, sizeof(inByte)/sizeof(byte), payload, &lenPay);
      if(payload[0] == 36){ // subscription request
        char msg[] = "test"; 
        //memcpy(payload, msg, length(msg));
        
        SendVESCPacket((uint8_t*)msg, strlen(msg));
      }
      if(payload[0] == 1){
        if(payload[1] == 1){
          digitalWrite(13,HIGH);
        } else if(payload[1] == 0){
          digitalWrite(13, LOW);
        }
      }
   }
   if(cnt++ > 100000){
      char msg[] = "Hello";
      SendVESCPacket((uint8_t*)msg, strlen(msg));
      cnt = 0;
   }
}
