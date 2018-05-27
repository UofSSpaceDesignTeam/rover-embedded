// Code to run and read data from science module instruments, including carousel, spectrometer, thermocouple, and moisture sensor 

#include "Robocluster.h"
#include "carousel.h"
#include "thermocouple.h"
#include "moisture_sensor.h"
//#include "carousel.h"

// Define Arduino Pins
const float IR_SENSOR = A2;
const float MOIST_SENSOR = A3;
const int CAROUSEL_MOTOR = 2;
const int CAROUSEL_DIRECTION = 3;
const int CAROUSEL_STEP = 4;
const int DETECTOR_FEED = 5;
const int EMITTER = 6;
const int HOME_SWITCH_READ = 7;
const int HOME_SWITCH_SEND = 8;
const int SAMPLE_LOADING = 15;

const int PROCESSOR_FREQUENCY = 16e6;
const int TOTAL_STEPS  = 400;

void wait() {
    while(1) {
        if("go") {
            break;
        }
    }
}

void setup()    {
    Serial.begin(115200);
    
    // Set pin I/O
    pinMode(EMITTER, OUTPUT);
    pinMode(CAROUSEL_MOTOR, OUTPUT);
    pinMode(CAROUSEL_STEP, OUTPUT);
    pinMode(CAROUSEL_DIRECTION, OUTPUT);
    pinMode(HOME_SWITCH_SEND, OUTPUT);
    pinMode(HOME_SWITCH_READ, INPUT);

    // Initialize send pins and ensure that nothing happens during initialization
    digitalWrite(CAROUSEL_STEP, LOW);
    digitalWrite(CAROUSEL_DIRECTION, LOW);
    digitalWrite(CAROUSEL_MOTOR, LOW);
    digitalWrite(EMITTER, LOW);
    digitalWrite(HOME_SWITCH_SEND, HIGH);

    // Wait for ping from cluster

    emitter_on();
    for (int i; i <= 5*PROCESSOR_FREQUENCY; i++)    {
        analyse_sample();
        // need a way to store this data
    }
    emitter_off();

    max.begin();

    char buffer[1024];
    sprintf(buffer, "{\"science\":\"%i\"}", 1);
    Publish(buffer);
    wait()
}

void loop() {
    while(digitalRead(HOME_SWITCH_READ) == FALSE) {
        sleep(0.5);
    }

    sprintf(char buffer[1024], "{\"go\":{\"%i\"]}", int 1);
    Publish(buffer);

    while("drill in ground") {
        read_temperature();
        read_moisture(analogRead(MOIST_SENSOR));
        delay(5);
    }

    while ("drill not home") {
        sleep(0.5);
    }

    // move carousel to sample deposit position
    // send sample position signal to drill
    // wait for drill deposit task
    // wait for go signal

    emitter_on();
    for (int i; i <= 5*PROCESSOR_FREQUENCY; i++)    {
        analyse_sample();
        // need a way to store this data
    }
    emitter_off();

    // Wait for store sample signal

    if ("store sample") {
        wait();
    }

    // move carousel to empty position
    // send empty position to drill
    // wait for drill empty task
    // wait for go signal

    wait();
}






