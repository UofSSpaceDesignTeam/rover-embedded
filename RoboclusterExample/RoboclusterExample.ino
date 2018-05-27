#include "Robocluster.h"

void blink(char *json_msg) {
    char *expected = "{\"test\": 42}";
    if (strcmp(json_msg, expected) == 0) {
        digitalWrite(13, HIGH);
    }
    Publish(json_msg);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT); //LED will blink when message received
  set_name("ExampleDevice");
  set_message_handler(blink);
}

void loop() {
  delay(1000);
  Publish("{\"test\":1234}");

}
