// Code to run and read data from science module instruments, including carousel, spectrometer, thermocouple, and moisture sensor

#include "Robocluster.h"
//#include "carousel.h"
#include "thermocouple.h"
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

//TODO: discuss order of tasks
//      one single command to analyse sample or one command per step

void move_carousel(char *data)  {
    int steps = atoi(data);
    for(int i=0; i<steps; i++) {
        step_motor(1, CAROUSEL_DIRECTION, CAROUSEL_STEP);
    }
}

void run_thermocouple(char *data)   {
    read_temperature();
}

void run_moisture_probe(char *data)  {
    read_moisture(MOIST_SENSOR);
}

void run_emitter(char *data)  {
    int d = atoi(data);
    if(d == 1) {
        emitter_on(EMITTER);
        digitalWrite(13, HIGH);
    }
    else if(d == 0)  {
        emitter_off(EMITTER);
    }
}

void take_reading(char *data) {
    analyse_sample();
}


void setup()    {
    Serial.begin(115200);
    a_max.begin();

    // Set pin I/O
    pinMode(EMITTER, OUTPUT);
    pinMode(CAROUSEL_STEP, OUTPUT);
    pinMode(CAROUSEL_DIRECTION, OUTPUT);
    pinMode(13, OUTPUT);

    // Initialize send pins and ensure that nothing happens during initialization
    digitalWrite(CAROUSEL_STEP, LOW);
    digitalWrite(CAROUSEL_DIRECTION, LOW);
    digitalWrite(EMITTER,HIGH);

    set_name("ScienceArduino");
    int num_msgs = 5;
    set_messages(num_msgs, "move_carousel", "run_thermocouple",       "run_moisture_probe", "run_emitter", "detector");
    set_callbacks(num_msgs, move_carousel, run_thermocouple,
    run_moisture_probe, run_emitter, take_reading);
    /* carousel_init(CAROUSEL_STEP, CAROUSEL_DIRECTION); */

    /* emitter_on(EMITTER); */
    /* int start_time = millis(); */
    /* while (millis()-start_time < 500)    { */
    /*     analyse_sample(); */
    /*     // need a way to store this data */
    /* } */
    /* emitter_off(EMITTER); */
    /*  */
    /* //max.begin(); */
    /*  */
    /* s_delay(100); */
    /*  */
    char buffer[BUFF_SIZE];
    sprintf(buffer, "{\"science_ready\":%i}", 1);
    Publish(buffer);
}

void loop() { 
    delay(1000);
    digitalWrite(13, LOW);
}






