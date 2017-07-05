/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

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

void setup() {
  door_lft.attach(11);  // attaches the servo on pin 9 to the servo object
  door_mid.attach(10);
  door_rgt.attach(9);

  lock(door_lft);
  delay(100);
  lock(door_mid);
  delay(100);
  lock(door_rgt);
  delay(100);

}

void loop() {

  lock(door_lft);
  delay(100);
  lock(door_mid);
  delay(100);
  lock(door_rgt);
  delay(100);

  delay(1000);

  unlock(door_lft);
  delay(100);
  unlock(door_mid);
  delay(100);
  unlock(door_rgt);
  delay(100);

  delay(1000);

}

