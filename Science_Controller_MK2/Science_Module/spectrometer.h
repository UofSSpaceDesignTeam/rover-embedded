// Code to run spectrometer, and to relay data back to main computer cluster

float spec_buffer[1024];

void setup()    {
    pinMode(emitter, OUTPUT);
    digitalWrite(emitter, LOW);
}

int detector()  {
    int intensity_voltage = analogRead(IRSensor);
    return intensity_voltage;
}

void emitter_on()  {
    digitalWrite(emitter, HIGH);
    // Check if this is right
}

void emitter_off()  {
    digitalWrite(emitter, LOW);
}


void analyse_sample() {
    while("Spectrometer active")    {
        sprintf(spec_buffer, "{\"data\":[\"%i\"]}", detector());
        Publish(spec_buffer));
    } 
}
