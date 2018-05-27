// Code to run spectrometer, and to relay data back to main computer cluster

float buffer[1024];
const float IRSensor = A2;
const int emitter = 6;

void setup()    {
    Serial.begin(115200);
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
    emitter_on();
    while("Spectrometer active")    {
        sprintf(buffer, "{\"data\":[\"%i\"]}", detector());
    } 
    emitter_off();
    while("wait")   {
        spectrometer.wait();
    }
}
