// Code to run and read data from science module instruments, including carousel, spectrometer, thermocouple, and moisture sensor

#include "Robocluster.h"
#include "carousel.h"
/* #include "thermocouple.h" */
#include "moisture_sensor.h"
#include "spectrometer.h"
//#include "carousel.h"

// Define Arduino Pins
#define IR_SENSOR  A2
#define MOIST_SENSOR  A3
#define CAROUSEL_MOTOR  2
#define CAROUSEL_DIRECTION  3
#define CAROUSEL_STEP  4
#define DETECTOR_FEED  5
#define EMITTER  6
#define HOME_SWITCH_READ  7
#define HOME_SWITCH_SEND  8
#define SAMPLE_LOADING  15

#define PROCESSOR_FREQUENCY 16e6
#define TOTAL_STEPS 400

int g_start_science = 0;

void enable_science(char *json_msg) {
    if (strcmp(json_msg, "{\"enable_science\": 1}") == 0) {
        digitalWrite(13, HIGH);
        g_start_science = 1;
    }
}

void wait() {
    while(!g_start_science) {
        s_delay(100);
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
    pinMode(13, OUTPUT);

    // Initialize send pins and ensure that nothing happens during initialization
    digitalWrite(CAROUSEL_STEP, LOW);
    digitalWrite(CAROUSEL_DIRECTION, LOW);
    digitalWrite(CAROUSEL_MOTOR, LOW);
    digitalWrite(EMITTER, LOW);
    digitalWrite(HOME_SWITCH_SEND, HIGH);

    // Wait for ping from cluster
    set_name("ScienceArduino");
    set_message_handler(enable_science);

    emitter_on(EMITTER);
    int start_time = millis();
    while (millis()-start_time < 500)    {
        analyse_sample();
        // need a way to store this data
    }
    emitter_off(EMITTER);

    /* max.begin(); */

    char buffer[BUFF_SIZE];
    sprintf(buffer, "{\"science\":\"%i\"}", 1);
    Publish(buffer);
    wait();
}

void loop() {
    /* while(digitalRead(HOME_SWITCH_READ) == 0) { */
    /*     s_delay(1); */
    /* } */
    char buffer[BUFF_SIZE];
    sprintf(buffer, "{\"go\":{\"%i\"]}", 1);
    Publish(buffer);

    while("drill in ground") {
        /* read_temperature(); */
        read_moisture(analogRead(MOIST_SENSOR));
        s_delay(100);
    }

    while ("drill not home") {
        s_delay(0.5);
    }

    // move carousel to sample deposit position
    // send sample position signal to drill
    // wait for drill deposit task
    // wait for go signal

    emitter_on(EMITTER);
    for (int i; i <= 5*PROCESSOR_FREQUENCY; i++)    {
        analyse_sample();
        // need a way to store this data
    }
    emitter_off(EMITTER);

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






