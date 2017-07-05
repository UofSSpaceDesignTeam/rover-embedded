#include <EEPROM.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#include "VESCPacket.h"

//#define DEV
#ifdef DEV

void print(char* str) {
  Serial.print(str);
}
void println(char *str) {
  Serial.println(str);
}

char DEV_sprintf_buffer[256];
#define printf(...) { \
  sprintf(DEV_sprintf_buffer, __VA_ARGS__); \
  print(DEV_sprintf_buffer); \
}

#else

void print(char* str) {}

void println(char* str) {}

#define printf(...)

#endif

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (250)

Adafruit_BNO055 bno = Adafruit_BNO055(-1, BNO055_ADDRESS_A);

/* calibration steps:
 * System: ???
 * Gyroscope: let it sit
 * Magnetometer: figure 8 motion
 * Accelerometer: place perpendicular to x, y, and z axis in both + and -
 */
adafruit_bno055_offsets_t bno_calibration = {
    65535, 65535, 65535,
    65534, 65534, 1,
    65500, 214, 413,
    1000,
    955
};

void print_calibration() {
  adafruit_bno055_offsets_t c;
  bno.getSensorOffsets(c);
  printf("    %u, %u, %u,\n    %u, %u, %u,\n    %u, %u, %u,\n    %u,\n    %u\n",
    c.accel_offset_x,
    c.accel_offset_y,
    c.accel_offset_z,
    c.gyro_offset_x,
    c.gyro_offset_y,
    c.gyro_offset_z,
    c.mag_offset_x,
    c.mag_offset_y,
    c.mag_offset_z,
    c.accel_radius,
    c.mag_radius
  );
}

void blink_led(byte* payload) {
  BlinkMessage msg = BlinkMessage(payload);
  if(msg.value == 1){ // arduino is little endian, network is big :(
    digitalWrite(13,HIGH);
  } else if(msg.value == 0){
    digitalWrite(13, LOW);
  }
}

void setup(void)
{
  Serial.begin(115200);

  /* Initialise the sensor */
  print("Waiting for BNO055");
  while(!bno.begin()) {
    print(".");
    delay(100);
  }

  /* Restore calibration */
  bno.setSensorOffsets(bno_calibration);

  print("\nInitialized!\n");

  delay(1000);
  init_msg_callbacks();
  subscribe(BLINK_LED, blink_led);
}


#define AVG_LENGTH 10

int8_t prev_index = -1;
imu::Vector<3> prev_accel[AVG_LENGTH];

imu::Vector<3> prev_avg(imu::Vector<3> prev[AVG_LENGTH]) {
  imu::Vector<3> avg = 0;
  for (int i = 0; i < AVG_LENGTH; i++) {
    avg[0] += prev[i][0];
    avg[1] += prev[i][1];
    avg[2] += prev[i][2];
  }
  avg[0] /= AVG_LENGTH;
  avg[1] /= AVG_LENGTH;
  avg[2] /= AVG_LENGTH;
  return avg;
}

void loop(void)
{
  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  imu::Vector<3> vec3;

  /* system calibration is a black box:
   * system = 0 -> yeah we're fucked
   * system = 1 -> it may work
   * system = 2 -> it might be wrong
   * system = 3 -> good to go
   */

  printf("system: %u, gyro %u, accel %u, mag %u\n", system, gyro, accel, mag);

  /* we like to live dangerously so lets disable the system check ;)
  if (system < 1) return;
  */

  if (accel >= 1) {
    vec3 = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

    float x = vec3.x();
    float y = vec3.y();
    float z = vec3.z();

    printf("acceleration: %f, %f, %f\n", x, y, z);

#ifndef DEV
    AccelerometerDataMessage msg = AccelerometerDataMessage(x, y, z);
    SendVESCPacket(&msg);
#endif
  }

  if (gyro >= 1 && mag >= 1) {
    vec3 = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    float heading = vec3.x();
    float pitch   = vec3.y();
    float roll    = vec3.z();

    printf("heading: %f, pitch %f, roll %f\n", heading, pitch, roll);

#ifndef DEV
    CompassDataMessage msg = CompassDataMessage(heading, pitch, roll);
    SendVESCPacket(&msg);
#endif
  }

  prev_index = (prev_index + 1) % 10;
  print_calibration();
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
