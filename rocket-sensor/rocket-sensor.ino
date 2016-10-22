#include "bmp180.h"
#include <avr/eeprom.h>
#define SENSORS_PRESSURE_SEALEVELHPA (1013.25F)

float max_alt = 0;



void setup() {
    bmpinit(BMP085_MODE_ULTRAHIGHRES);
}

void loop() {
    float pressure = getPressure();
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    float altitude = pressureToAltitude(seaLevelPressure, pressure/100);
    if(altitude > max_alt) {
        max_alt = altitude;
        eeprom_write_float(0, max_alt);
    }

    delay(500);

}
