/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
#include <string.h>
#include "VESCPacket.h"

Servo door_lft;
Servo door_mid;
Servo door_rgt;

void lock(Servo servo) {  
  // Poor connection means only sweeps work properly
  for (int pos = 60; pos <= 95; pos += 1) {
    servo.write(pos);
    delay(20);
  }
}

void unlock(Servo servo) {
  for (int pos = 90; pos <= 166; pos += 1) {
    servo.write(pos);
    delay(5);
  }
}

void door_control(byte* payload) {
  DoorControl msg = DoorControl(payload);
  if(msg.value == 0) {
    unlock(door_lft);
  }
  else if(msg.value == 1) {
    unlock(door_mid);
  }
  else if(msg.value == 2) {
    unlock(door_rgt);
  }
  else {
    lock(door_lft);
    lock(door_mid);
    lock(door_rgt);
  }
}

void setup() {
  door_lft.attach(11);  // attaches the servo on pin 9 to the servo object
  door_mid.attach(10);
  door_rgt.attach(9);

  init_msg_callbacks();
  subscribe(DOOR_CONTROL, door_control);

  lock(door_lft);
  lock(door_mid);
  lock(door_rgt);
  
}

void loop() {
  delay(1000);
}

