#include <VESCPacket.h>
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
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
      ReadVESCMessage(inByte);
      UnpackPayload(inByte, sizeof(inByte)/sizeof(byte), payload, &lenPay);
      if(payload[0] == 36){
        char msg[] = {'t','e','s','t'};
        //memcpy(payload, msg, length(msg));
        
        SendVESCPacket((uint8_t*)msg, sizeof(msg)/sizeof(char));
        }
      if(payload[0] == 1){
        if(payload[1] == 1){
          digitalWrite(13,HIGH);
          }
        else if(payload[1] == 0){
          digitalWrite(13, LOW);
          }
        }
    }
    else if(cnt++ > 2000){
      char msg[] = {'H','E','L','L','O'};
      SendVESCPacket((uint8_t*)msg, sizeof(msg)/sizeof(char));
      cnt = 0;
      }
   
      
}
