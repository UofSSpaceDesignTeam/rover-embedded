// Code to read temperature data from thermocouple, and relay this data to the main computer cluster
// Needs calibration

#include "Adafruit_MAX31856.h"
#include "Robocluster.h"

Adafruit_MAX31856 max = Adafruit_MAX31856(9, 10, 11, 12);
max.begin();

char th_buffer[BUFF_SIZE];

// This isn't going to work properly right now
char* parse_fault()  {
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
        // sprintf(th_buffer, "{\"thermocouple_error\":[\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\"]}", parse_fault());
    }
    else    {
        memset(th_buffer, 0, BUFF_SIZE);
        sprintf(th_buffer, "{\"thermocouple_data\":[\"%s\",\"%s\"]}", max.readCJTemperature(), max.readThermocoupleTemperature());    
        Publish(th_buffer);
    }
    s_delay(1000);
}
