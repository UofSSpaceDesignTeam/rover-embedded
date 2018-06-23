// Code to run carousel and relay data to main computer

#include "Robocluster.h"

char car_buffer[BUFF_SIZE];
#define full_rotation 400 // steps

void step_motor(int direction, int direction_pin, int step_pin)    {
    digitalWrite(direction_pin, direction);
    digitalWrite(step_pin, HIGH);
    s_delay(20);
    digitalWrite(step_pin, LOW);
}

void carousel_init(int step_pin, int direction_pin)    {
    digitalWrite(motor_arduino_pin, HIGH);
    while("home switch false")  {
        step_motor("ccw", direction_pin, step_pin);
    }

}
