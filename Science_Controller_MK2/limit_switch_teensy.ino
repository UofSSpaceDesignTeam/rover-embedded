// Code to run all limit switches off of a Teensy board, and relay this data to an Arduino Uno
  #include "Robocluster.h"

  // Drill Stage 1 Top Switch
  const int DRILL_1_TOP_SEND = 1;
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
  
  char buffer[1024];

void setup() {
    Serial.begin(115000);
    set_name("Limit Switch Array")
    pinMode(ARDUINO_SEND, OUTPUT);
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
}

void drill_1_top(){
    if (digitalRead(DRILL_1_TOP_READ) == HIGH){
        return 'A'
    }
    else if (digitalRead(DRILL_1_TOP_READ) == LOW){
        return 'H'
    }
    else{
        return 'E'
    }
}

void drill_2_top(){
    if (digitalRead(DRILL_2_TOP_READ) == HIGH){
        return 'A'
    }
    else if (digitalRead(DRILL_2_TOP_READ) == LOW){
        return 'H'
    }
    else{
        return 'E'
    }
}

void drill_1_bottom(){
    if (digitalRead(DRILL_1_BOTTOM_READ) == HIGH){
        return 'A'
    }
    else if (digitalRead(DRILL_1_BOTTOM_READ) == LOW){
        return 'H'
    }
    else{
        return 'E'
    }
}

void drill_2_bottom(){
    if (digitalRead(DRILL_2_BOTTOM_READ) == HIGH){
        return 'A'
    }
    else if (digitalRead(DRILL_2_BOTTOM_READ) == LOW){
        return 'H'
    }
    else{
        return 'E'
    }
}

void carousel_sample(){
    if (digitalRead(CAROUSEL_SAMPLE_READ) == HIGH){
        return 'A'
    }
    else if (digitalRead(CAROUSEL_SAMPLE_READ) == LOW){
        return 'H'
    }
    else{
        return 'E'
    }
}

void carousel_dump(){
    if (digitalRead(CAROUSEL_DUMP_READ) == HIGH){
        return 'A' // away
    }
    else if (digitalRead(CAROUSEL_DUMP_READ) == LOW){
        return 'H' // here
    }
    else{
        return 'E' // error
    }
}


void loop() {
    sprintf(buffer, "{\"data\":[%c,%c,%c,%c,%c,%c]}",drill_1_top(), drill_1_bottom(), drill_2_top(), drill_2_bottom(), carousel_sample(), carousel_dump());
    Publish(buffer);
    delay(10);
}