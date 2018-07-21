// Code to run spectrometer, and to relay data back to main computer cluster

#include "Robocluster.h"
char spec_buffer[BUFF_SIZE];

float detector(int IRSensor)  {
    float intensity_voltage = analogRead(IRSensor);
    return intensity_voltage;
}

void emitter_on(int emitter)  {
    digitalWrite(emitter, LOW);
    // Check if this is right
}

void emitter_off(int emitter)  {
    digitalWrite(emitter, HIGH);
}


void analyse_sample() {
    sprintf(spec_buffer, "{\"spectrometer_data\":%f}", detector(A1));
    Publish(spec_buffer);
}
