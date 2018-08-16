// Code to run carousel and relay data to main computer

#include "Robocluster.h"

char car_buffer[BUFF_SIZE];
#define full_rotation 400 // steps

void step_motor(int direction, int direction_pin, int step_pin)    {
    digitalWrite(direction_pin, direction);
    digitalWrite(step_pin, HIGH);
    delay(20);
    digitalWrite(step_pin, LOW);
}

void carousel_init(int step_pin, int direction_pin)    {
    while("home switch false")  {
        step_motor(-1, direction_pin, step_pin);
    }

}

void motor_on(int pin)  {
    digitalWrite(pin, HIGH);
}

void motor_off(int pin)  {
    digitalWrite(pin, LOW);
}
