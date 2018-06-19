// Code to run all limit switches off of a Teensy board, and relay this data to the main computer cluster

#include "Robocluster.h"

  // Drill Stage 1 Top Switch
  #define DRILL_1_TOP_SEND 1
  #define DRILL_1_TOP_READ 2

  // Drill Stage 1 Bottom Switch
  #define DRILL_1_BOTTOM_SEND 3
  #define DRILL_1_BOTTOM_READ 4

  // Drill Stage 2 Top Switch
  #define DRILL_2_TOP_SEND 5
  #define DRILL_2_TOP_READ 6

  // Drill Stage 2 Bottom Switch
  #define DRILL_2_BOTTOM_SEND 7
  #define DRILL_2_BOTTOM_READ 8

  // Carousel Sample Holder Switch
  #define CAROUSEL_SAMPLE_SEND 9
  #define CAROUSEL_SAMPLE_READ 10

  // Carousel Sample Dump Switch
  #define CAROUSEL_DUMP_SEND 11
  #define CAROUSEL_DUMP_READ 12
  
  char buffer[BUFF_SIZE];

void setup() {
    Serial.begin(115000);
    //set_name("Limit Switch Array")
    pinMode(DRILL_1_TOP_SEND, OUTPUT);
    pinMode(DRILL_1_TOP_READ, INPUT);
    pinMode(DRILL_2_TOP_SEND, OUTPUT);
    pinMode(DRILL_2_TOP_READ, INPUT);
    pinMode(DRILL_1_BOTTOM_SEND, OUTPUT);
    pinMode(DRILL_1_BOTTOM_READ, INPUT);
    pinMode(DRILL_2_BOTTOM_SEND, OUTPUT);
    pinMode(DRILL_2_BOTTOM_READ, INPUT);
    pinMode(CAROUSEL_SAMPLE_SEND, OUTPUT);
    pinMode(CAROUSEL_SAMPLE_READ, INPUT);
    pinMode(CAROUSEL_DUMP_SEND, OUTPUT);
    pinMode(CAROUSEL_DUMP_READ, INPUT);
    digitalWrite(DRILL_1_TOP_SEND, HIGH);
    digitalWrite(DRILL_2_TOP_SEND, HIGH);
    digitalWrite(DRILL_1_BOTTOM_SEND, HIGH);
    digitalWrite(DRILL_2_BOTTOM_SEND, HIGH);
    digitalWrite(CAROUSEL_SAMPLE_SEND, HIGH);
    digitalWrite(CAROUSEL_DUMP_SEND, HIGH);
}

bool drill_1_top(){
    if (digitalRead(DRILL_1_TOP_READ) == HIGH){
        return false;
    }
    else if (digitalRead(DRILL_1_TOP_READ) == LOW){
        return true;
    }
}
bool drill_2_top(){
    if (digitalRead(DRILL_2_TOP_READ) == HIGH){
        return false;
    }
    else if (digitalRead(DRILL_2_TOP_READ) == LOW){
        return true;
    }
}
bool drill_1_bottom(){
    if (digitalRead(DRILL_1_BOTTOM_READ) == HIGH){
        return false;
    }
    else if (digitalRead(DRILL_1_BOTTOM_READ) == LOW){
        return true;
    }
}
bool drill_2_bottom(){
    if (digitalRead(DRILL_2_BOTTOM_READ) == HIGH){
        return false;
    }
    else if (digitalRead(DRILL_2_BOTTOM_READ) == LOW){
        return true;
    }
}
bool carousel_sample(){
    if (digitalRead(CAROUSEL_SAMPLE_READ) == HIGH){
        return false;
    }
    else if (digitalRead(CAROUSEL_SAMPLE_READ) == LOW){
        return true;
    }
}
bool carousel_dump(){
    if (digitalRead(CAROUSEL_DUMP_READ) == HIGH){
        return false; // away
    }
    else if (digitalRead(CAROUSEL_DUMP_READ) == LOW){
        return true; // here
    }
}

void loop() {
    memset(buffer, 0, BUFF_SIZE);
    sprintf(buffer, "{\"science_limit_switches\":[%i,%i,%i,%i,%i,%i]}", drill_1_top(), drill_1_bottom(), drill_2_top(), drill_2_bottom(), carousel_sample(), carousel_dump());
    Publish(buffer);   // Used for debugging
    //Publish(buffer);
    delay(200);
}
