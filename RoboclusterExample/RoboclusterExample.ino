#include "Robocluster.h"

void blink(char *data) {
    char *expected = "42";
    Publish(data);
    if (strcmp(data, expected) == 0) {
    }
}

void dynamic_delay(char *data) {
    int dt = atoi(data);
    digitalWrite(13, HIGH);
    delay(dt);
    digitalWrite(13, LOW);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT); //LED will blink when message received
  set_name("ExampleDevice");
  int n_msgs = 2; // REMEMBER TO SET THIS TO THE RIGHT NUMBER OF MESSAGES :)
  set_messages(n_msgs, "test", "delay");
  set_callbacks(n_msgs, blink, dynamic_delay);
}

void loop() {
  delay(1000);
  /* Publish("{\"test\":1234}"); */

}
