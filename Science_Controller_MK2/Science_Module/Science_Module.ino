// Code to run and read data from science module instruments, including carousel, spectrometer, thermocouple, and moisture sensor

#include "Robocluster.h"
#include "carousel.h"
#include "thermocouple.h"
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

int g_start_science = 0;

void enable_science(char *json_msg) {
    char *expected = "{\"enable_science\": 1}";
    if (strcmp(json_msg, expected) == 0) {
        g_start_science = 1;
    }
}

void wait() {
    while(!g_start_science) {
        s_delay(100);
    }
    g_start_science = 0;
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

    set_name("ScienceArduino");
    set_message_handler(enable_science);

    // home carousel

    emitter_on(EMITTER);
    int start_time = millis();
    while (millis()-start_time < 500)    {
        analyse_sample();
        // need a way to store this data
    }
    emitter_off(EMITTER);

    max.begin();

    s_delay(100);

    char buffer[BUFF_SIZE];
    sprintf(buffer, "{\"science_ready\":%i}", 1);
    Publish(buffer);
}

void loop() { 
    wait();
    char buffer[BUFF_SIZE];
    sprintf(buffer, "{\"take_sample\":%i}", 1);
    Publish(buffer);   
    wait();

    while(1) {
        s_delay(100);
        read_temperature();
        read_moisture(analogRead(MOIST_SENSOR));
        if (g_start_science)    {
            g_start_science = 0;
            break;
        }
    }

    wait();

    memset(buffer, 0, BUFF_SIZE);
    sprintf(buffer, "{\"science_ready\":%i}", 0);
    Publish(buffer);
    s_delay(100);
    
    // move carousel to sample deposit position

    memset(buffer, 0, BUFF_SIZE);
    sprintf(buffer, "{\"carousel_position\":\"%s\"}", "sample");
    Publish(buffer);
    memset(buffer, 0, BUFF_SIZE);
    sprintf(buffer, "{\"science_ready\":%i}", 1);
    Publish(buffer);
    memset(buffer, 0, BUFF_SIZE);
    sprintf(buffer, "{\"deposit_sample\":%i}", 1);
    Publish(buffer);
    
    wait();

    memset(buffer, 0, BUFF_SIZE);
    sprintf(buffer, "{\"science_ready\":%i}", 0);
    Publish(buffer);

    // move carousel to sample analysis position

    emitter_on(EMITTER);
    int start_time = millis();
    while (millis()-start_time < 500)    {
        analyse_sample();
        // need a way to store this data
    }
    emitter_off(EMITTER);

    // there needs to be something here to push the data to the webui and to allow for human input of some sort, hopefully
    // Wait for store sample signal

    if ("store sample") {
        // move carousel home
        wait();
    }

    // move carousel to empty position

    memset(buffer, 0, BUFF_SIZE);
    sprintf(buffer, "{\"carousel_position\":\"%s\"}", "empty");
    Publish(buffer);
    memset(buffer, 0, BUFF_SIZE);
    sprintf(buffer, "{\"empty_sample\":%i}", 1);
    Publish(buffer);

    wait();

    // move carousel home

    wait();
}






