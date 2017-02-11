#include "VESCPacket.h"
#include <string.h>

void blink_led(byte* payload) {
  BlinkMessage msg = BlinkMessage(payload);
  if(msg.value == 1){ // arduino is little endian, network is big :(
    digitalWrite(13,HIGH);
  } else if(msg.value == 0){
    digitalWrite(13, LOW);
  }
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
