#include <EEPROM.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#include "VESCPacket.h"
#include "vector.h"
#include "math.h"

/* #define DEV */
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
  init_msg_callbacks();

  Serial.begin(115200);

  /* Initialise the sensor */
  print("Waiting for BNO055");
  while(!bno.begin()) {
    print(".");
    delay(100);
  }

  /* Restore calibration */
  bno.setSensorOffsets(bno_calibration);
  /* bno.setMode(Adafruit_BNO055::OPERATION_MODE_COMPASS); */

  print("\nInitialized!\n");

  delay(1000);

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

  //printf("system: %u, gyro %u, accel %u, mag %u\n", system, gyro, accel, mag);

  /* we like to live dangerously so lets disable the system check ;)
  if (system < 1) return;
  */

  if (accel >= 1) {
    vec3 = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

    float x = vec3.x();
    float y = vec3.y();
    float z = vec3.z();

    /* printf("acceleration: %f, %f, %f\n", x, y, z); */

#ifndef DEV
    //AccelerometerDataMessage msg = AccelerometerDataMessage(x, y, z);
    //SendVESCPacket(&msg);
#endif
  }

  if (gyro >= 1 && mag >= 1 && accel >= 1) {
    vec3 = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Quaternion quat = bno.getQuat();

    float heading = vec3.x();
    float roll   = vec3.y();
    float pitch    = vec3.z();

    /* printf("Quaternion x: %f\t y: %f\t z: %f\t w: %f\n", quat.x(), quat.y(), quat.z(), quat.w()) */
    float qr = quat.w();
    float qi = quat.x();
    float qj = quat.y();
    float qk = quat.z();

     /* float s = sqrt(qr*qr+qi*qi+qj*qj+qk*qk); */
     /* qr /= s; */
     /* qi /= s; */
     /* qj /= s; */
     /* qk /= s; */
    /* printf("Normalized Quaternion r: %f\t i: %f\t j: %f\t k: %f\n", qr, qi, qj, qk) */
    /* float s = 1; */
    /* double RotationMat[3][3] = { */
    /*     {1-2*s*(qj*qj+qk*qk), 2*s*(qi*qj-qk*qr), 2*s*(qi*qk+qj*qr)}, */
    /*     {2*s*(qi*qj+qk*qr), 1-2*s*(qi*qi+qk*qk), 2*s*(qj*qk-qi*qk)}, */
    /*     {2*s*(qi*qk-qj*qr), 2*s*(qj*qk+qi*qr), 1-2*s*(qi*qi+qj*qj)} */
    /* }; */

    float psi = radians(heading);
    float theta = radians(pitch);
    float phi = radians(roll);
    /* double eulerRM[3][3] = { */
    /*     {cos(theta)*cos(psi), sin(psi)*cos(theta), -sin(theta)}, */
    /*     {-cos(phi)*sin(psi)+sin(phi)*sin(theta)*cos(phi), cos(phi)*cos(psi)+sin(phi)*sin(psi), sin(phi)*cos(theta)}, */
    /*     {sin(phi)*sin(psi)+cos(phi)*sin(theta)*cos(psi), -sin(phi)*cos(psi)+cos(phi)*sin(theta)*sin(psi), cos(phi)*cos(theta)} */
    /* }; */
    /* double eulerRMT[3][3] = { */
    /*     {cos(theta)*cos(psi), sin(phi)*sin(theta)*cos(psi)-cos(phi)*sin(psi), cos(phi)*sin(theta)*cos(psi)+sin(phi)*sin(psi)}, */
    /*     {cos(theta)*sin(psi), sin(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi), cos(phi)*sin(theta)*sin(psi)-sin(phi)*cos(psi)}, */
    /*     {-sin(theta), sin(phi)*cos(theta), cos(phi)*cos(theta)} */
    /* }; */
    float eulerRMYaw[3][3] = {
        {cos(psi), -sin(psi), 0},
        {sin(psi), cos(psi), 0},
        {0,0,1}
    };
    /* TRANSPOSE_MATRIX_3X3(eulerRMT, eulerRM); */
/* double rm[3][3]; */
/*  */
/* rm[0][0] = quat.w()*quat.w() + quat.x()*quat.x() - quat.y()*quat.y() - quat.z()*quat.z(); */
/* rm[0][1] = 2*quat.x()*quat.y() - 2*quat.w()*quat.z(); */
/* rm[0][2] = 2*quat.x()*quat.z() + 2*quat.w()*quat.y(); */
/* rm[1][0] = 2*quat.x()*quat.y() + 2*quat.w()*quat.z(); */
/* rm[1][1] = quat.w()*quat.w() - quat.x()*quat.x() + quat.y()*quat.y() - quat.z()*quat.z(); */
/* rm[1][2] = 2*quat.y()*quat.z() - 2*quat.w()*quat.x(); */
/* rm[2][0] = 2*quat.x()*quat.z() - 2*quat.w()*quat.y(); */
/* rm[2][1] = 2*quat.y()*quat.z() + 2*quat.w()*quat.x(); */
/* rm[2][2] = quat.w()*quat.w() - quat.x()*quat.x() - quat.y()*quat.y() + quat.z()*quat.z(); */
/*  */
    double InverseRotMat[3][3];
    double determinant;
    /* INVERT_3X3(InverseRotMat, determinant, eulerRMYaw); */
    /* print("Euler RM:\n"); */
    /* printf("%f\t %f\t %f\n", eulerRM[0][0], eulerRM[0][1], eulerRM[0][2]); */
    /* printf("%f\t %f\t %f\n", eulerRM[1][0], eulerRM[1][1], eulerRM[1][2]); */
    /* printf("%f\t %f\t %f\n", eulerRM[2][0], eulerRM[2][1], eulerRM[2][2]); */
    /* printf("Determinant %f\n", determinant); */
    /* printf("Inverse Euler RM:\n"); */
    /* printf("%f\t %f\t %f\n", InverseRotMat[0][0], InverseRotMat[0][1], InverseRotMat[0][2]); */
    /* printf("%f\t %f\t %f\n", InverseRotMat[1][0], InverseRotMat[1][1], InverseRotMat[1][2]); */
    /* printf("%f\t %f\t %f\n", InverseRotMat[2][0], InverseRotMat[2][1], InverseRotMat[2][2]); */
    double axe = eulerRMYaw[0][0]*accel.x() + eulerRMYaw[0][1]*accel.y() + eulerRMYaw[0][2]*accel.z();
    double aye = eulerRMYaw[1][0]*accel.x() + eulerRMYaw[1][1]*accel.y() + eulerRMYaw[1][2]*accel.z();
    double aze = eulerRMYaw[2][0]*accel.x() + eulerRMYaw[2][1]*accel.y() + eulerRMYaw[2][2]*accel.z();

    /* double ax = rm[0][0]*0 + rm[0][1]*0 + rm[0][2]*9.8; */
    /* double ay = rm[1][0]*0 + rm[1][1]*0 + rm[1][2]*9.8; */
    /* double az = rm[2][0]*0 + rm[2][1]*0 + rm[2][2]*9.8; */
    printf("Absolute Accel: ae: %f\t an: %f\t ad: %f\n", axe, aye, aze);
    /* printf("Relative Accel: ax: %f\t ay: %f\t az: %f\n", ax, ay, az); */
    /* printf("Read Accel: ax: %f\t ay: %f\t az: %f\n", accel.x(), accel.y(), accel.z()); */


    printf("heading: %f, pitch %f, roll %f\n", heading, pitch, roll);

#ifndef DEV
    CompassDataMessage msg = CompassDataMessage(heading, pitch, roll);
    SendVESCPacket(&msg);
    AccelerometerDataMessage amsg = AccelerometerDataMessage(axe, aye, aze);
    SendVESCPacket(&amsg);
#endif
  }

  prev_index = (prev_index + 1) % 10;
  //print_calibration();
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
