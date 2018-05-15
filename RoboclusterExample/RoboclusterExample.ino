#include "Robocluster.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT); //LED will blink when message received
  set_name("ExampleDevice");
}

void loop() {
  delay(1000);
  Publish("{\"test\":1234}");

}
