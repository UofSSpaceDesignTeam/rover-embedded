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
      int bytes_read = ReadVESCPacket(inByte, 256);
      UnpackMessage(inByte, bytes_read, payload, &lenPay);
      if(payload[0] == REQ_SUBSCRIPTION){ // subscription request
        char msg[] = "blink"; 
        SendVESCPacket(36, msg, strlen(msg));
      }
      if(payload[0] == BLINK_LED){
        if(*(int*)(payload + 1) >= 1){ // arduino is little endian, network is big :(
          digitalWrite(13,HIGH);
        } else if(*(int*)(payload + 1) == 0){
          digitalWrite(13, LOW);
        }
      } else {
        inByte[bytes_read+1] = '\0';
        Serial.println((char*)inByte);
      }
   }
   if(cnt++ > 100000){
      //char *msg = "test";
      //SendVESCPacket(37, (void*)msg, strlen(msg));
      cnt = 0;
   }
}
