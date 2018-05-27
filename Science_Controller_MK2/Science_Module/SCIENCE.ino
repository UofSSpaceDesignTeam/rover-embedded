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
const int DRILL_DOWN = 13;
const int DRILL_UP = 14;
const int SAMPLE_LOADING = 15;

const int TOTAL_STEPS  = 400;

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

    // Wait for ping from main cluster
}

void loop() {
    // wait for "go" signal
        // This means drill and carousel are home, spec is off
    
    while(digitalRead(HOME_SWITCH_READ) == FALSE)   {
        sleep(0.5);
    }

    max.begin();

    // send go to drill

    while("drill in ground")  {
        read_temperature();
        read_moisture(analogRead(MOIST_SENSOR));
        delay(5);
    }

    while ("drill not home")   {
        sleep(0.5);
    }

    // move carousel to sample deposit position
    // send sample position signal to drill
    // wait for drill deposit task
    
    
    

}






