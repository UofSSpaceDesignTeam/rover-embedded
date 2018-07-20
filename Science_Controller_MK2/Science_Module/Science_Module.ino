// Code to run and read data from science module instruments, including carousel, spectrometer, thermocouple, and moisture sensor

#include "Robocluster.h"
#include "carousel.h"
//#include "thermocouple.h"
#include "moisture_sensor.h"
#include "spectrometer.h"
#include "carousel.h"

// Define Arduino Pins
#define IR_SENSOR  A1
#define MOIST_SENSOR  A2
#define CAROUSEL_DIRECTION  6
#define CAROUSEL_STEP  7
#define DETECTOR_FEED  8
#define EMITTER  5

int g_start_science = 1;

void enable_science(char *json_msg) {
    char *expected = "{\"enable_science\": 1}";
    if (strcmp(json_msg, expected) == 0) {
        g_start_science = 1;
    }
}

/*
void wait() {
    while(!g_start_science) {
        s_delay(100);
    }
}
*/

void setup()    {
    Serial.begin(115200);

    // Set pin I/O
    pinMode(EMITTER, OUTPUT);
    pinMode(CAROUSEL_STEP, OUTPUT);
    pinMode(CAROUSEL_DIRECTION, OUTPUT);
    pinMode(13, OUTPUT);

    // Initialize send pins and ensure that nothing happens during initialization
    digitalWrite(CAROUSEL_STEP, LOW);
    digitalWrite(CAROUSEL_DIRECTION, LOW);
    digitalWrite(EMITTER, LOW);

    set_name("ScienceArduino");
    set_message_handler(enable_science);

    carousel_init(CAROUSEL_STEP, CAROUSEL_DIRECTION);

    emitter_on(EMITTER);
    int start_time = millis();
    while (millis()-start_time < 500)    {
        analyse_sample();
        // need a way to store this data
    }
    emitter_off(EMITTER);

    //max.begin();

    s_delay(100);

    char buffer[BUFF_SIZE];
    sprintf(buffer, "{\"science_ready\":%i}", 1);
    Publish(buffer);
}

void loop() { 

}






