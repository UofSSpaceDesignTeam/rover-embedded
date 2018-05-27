// Code to read temperature data from thermocouple, and relay this data to the main computer cluster
// Needs calibration

#include "Adafruit_MAX31856.h"

Adafruit_MAX31856 max = Adafruit_MAX31856(9, 10, 11, 12);

char th_buffer[1024];

// This isn't going to work properly right now
void parse_fault()  {
        if (max.readFault() & MAX31856_FAULT_CJRANGE)   {
            return("Cold Junction Range Fault");
        }
        else    {
            return(" ");
        }

        if (max.readFault() & MAX31856_FAULT_TCRANGE)   {
            return("Thermocouple Range Fault");
        }
        else    {
            return(" ");
        }

        if (max.readFault() & MAX31856_FAULT_CJHIGH)    {
            return("Cold Junction High Fault");
        }
        else    {
            return(" ");
        }

        if (max.readFault() & MAX31856_FAULT_CJLOW) {
            return("Cold Junction Low Fault");
        }
        else    {
            return(" ");
        }

        if (max.readFault() & MAX31856_FAULT_TCHIGH)    {
            return("Thermocouple High Fault");
        }
        else    {
            return(" ");
        }

        if (max.readFault() & MAX31856_FAULT_TCLOW) {
            return("Thermocouple Low Fault");
        }
        else    {
            return(" ");
        }

        if (max.readFault() & MAX31856_FAULT_OVUV)  {
            return("Over/Under Voltage Fault");
        }
        else    {
            return(" ");
        }

        if (max.readFault() & MAX31856_FAULT_OPEN)  {
            return("Thermocouple Open Fault");
        }
        else    {
            return(" ");
        }
}

void read_temperature() {
    if (max.readFault()) {
        // Won't work properly right now 
        sprintf(th_buffer, "{\"data\":[\"%c\", \"%c\", \"%c\", \"%c\", \"%c\", \"%c\"]}", parse_fault());
    }
    else    {
        sprintf(th_buffer, "{\"data\":[\"%c\",\"%c\"]}", max.readCJTemperature(), max.readThermocoupleTemperature());    
    }
    Publish(buffer);
}
