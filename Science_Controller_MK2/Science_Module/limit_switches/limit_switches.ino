// Code to run all limit switches off of a Teensy board, and relay this data to the main computer cluster

#include "Robocluster.h"

  // Drill Stage 1 Top Switch
  #define DRILL_1_TOP_SEND 1;
  const int DRILL_1_TOP_READ = 2;

  // Drill Stage 1 Bottom Switch
  const int DRILL_1_BOTTOM_SEND = 3;
  const int DRILL_1_BOTTOM_READ = 4;

  // Drill Stage 2 Top Switch
  const int DRILL_2_TOP_SEND = 5;
  const int DRILL_2_TOP_READ = 6;

  // Drill Stage 2 Bottom Switch
  const int DRILL_2_BOTTOM_SEND = 7;
  const int DRILL_2_BOTTOM_READ = 8;

  // Carousel Sample Holder Switch
  const int CAROUSEL_SAMPLE_SEND = 9;
  const int CAROUSEL_SAMPLE_READ = 10;

  // Carousel Sample Dump Switch
  const int CAROUSEL_DUMP_SEND = 11;
  const int CAROUSEL_DUMP_READ = 12;
  
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

char drill_1_top(){
    if (digitalRead(DRILL_1_TOP_READ) == HIGH){
        return False;
    }
    else if (digitalRead(DRILL_1_TOP_READ) == LOW){
        return True;
    }
}
char drill_2_top(){
    if (digitalRead(DRILL_2_TOP_READ) == HIGH){
        return False;
    }
    else if (digitalRead(DRILL_2_TOP_READ) == LOW){
        return True;
    }
}

char drill_1_bottom(){
    if (digitalRead(DRILL_1_BOTTOM_READ) == HIGH){
        return False;
    }
    else if (digitalRead(DRILL_1_BOTTOM_READ) == LOW){
        return True;
    }
}

char drill_2_bottom(){
    if (digitalRead(DRILL_2_BOTTOM_READ) == HIGH){
        return False;
    }
    else if (digitalRead(DRILL_2_BOTTOM_READ) == LOW){
        return True;
    }
}

char carousel_sample(){
    if (digitalRead(CAROUSEL_SAMPLE_READ) == HIGH){
        return False;
    }
    else if (digitalRead(CAROUSEL_SAMPLE_READ) == LOW){
        return True;
    }
}

char carousel_dump(){
    if (digitalRead(CAROUSEL_DUMP_READ) == HIGH){
        return False; // away
    }
    else if (digitalRead(CAROUSEL_DUMP_READ) == LOW){
        return True; // here
    }
}

void loop() {
    sprintf(buffer, "{\"science_limit_switches\":[\"%c\",\"%c\",\"%c\",\"%c\",\"%c\",\"%c\"]}", drill_1_top(), drill_1_bottom(), drill_2_top(), drill_2_bottom(), carousel_sample(), carousel_dump());
    //Serial.println(buffer);   // Used for debugging
    Publish(buffer);
    delay(10);
}
