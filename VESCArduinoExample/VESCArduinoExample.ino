#include "VESCPacket.h"
#include <string.h>
byte inByte[256];
byte payload[256];
int lenPay = 0;

void blink_led(byte* payload) {
  if(*(int*)(payload + 1) >= 1){ // arduino is little endian, network is big :(
    digitalWrite(13,HIGH);
  } else if(*(int*)(payload + 1) == 0){
    digitalWrite(13, LOW);
  }
}

void serialEvent() {
  noInterrupts(); // make sure we can only handle one message at a time.
  if(Serial.available()) {
      int bytes_read = ReadVESCPacket(inByte, 256);
      UnpackMessage(inByte, bytes_read, payload, &lenPay);
      if(payload[0] >= 0 && payload[0] <= NR_MSGS) {
        if(msg_callbacks[payload[0]] != NULL) {
          msg_callbacks[payload[0]](payload);
        }
      }
  }
  interrupts();
}

void setup() {
  
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  init_msg_callbacks();
  subscribe(BLINK_LED, blink_led);
  Serial.begin(115200);
  while(!Serial);
}

void loop() {

}
