/***********************************************************************************************************
/* Lynxmotion BotBoarduino LIDAR Lite Sweep Scan
/* Version: 1.0.0
/* By: scharette
/* Date: 2015-03-31
/* 
/* The LIDAR lite is attached to VCC (+5 V DC), GND, SDA and SCL.
 *  
 */
/* 
/* This examples uses the Wire library.
/*
/* Hardware used:
 * 
/* BotBoarduino: http://www.robotshop.com/en/lynxmotion-botboarduino-robot-controller.html?product-code=RB-Lyn-363
/* LIDAR-Lite Laser Rangefinder: http://www.robotshop.com/en/lidar-lite-laser-rangefinder-pulsedlight.html?product-code=RB-Pli-01
/* HS-645MG servo motor: http://www.robotshop.com/en/hitec-hs645mg-servo-motor.html?product-code=RB-Hit-29
/* (alternative) HS-422 servo motor: http://www.robotshop.com/en/hitec-hs422-servo-motor.html?product-code=RB-Hit-27
/* Bracket to hold the LIDAR: http://www.robotshop.com/en/aluminum-l-connector-bracket-asb-06.html?product-code=RB-Lyn-83
/* or this one: http://www.robotshop.com/en/lynxmotion-multi-purpose-sensor-housing.html?product-code=RB-Lyn-75
/* Bracket to hold the servo motor: http://www.robotshop.com/en/lynxmotion-aluminum-multi-purpose-servo.html?product-code=RB-Lyn-81
 *  
 * Stepper motor library: https://github.com/laurb9/StepperDriver  
 *
/*
/* *: Please note that while we used a HS-645MG in our example, the HS-422 and almost any other standard RC servo motor could perform this task.
/*
/**********************************************************************************************************/

#include <Wire.h>
#include "A4988.h"
#include "VESCPacket.h"

#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.

#define MOTOR_STEPS 200
#define DIR 8
#define STEP 9
#define MS1 10
 #define MS2 11
 #define MS3 12
 A4988 stepper(MOTOR_STEPS, DIR, STEP, MS1, MS2, MS3);

#define MICROSTEPS 16

#include <Servo.h>

Servo myservo;

double pos = 0;         // Position of the servo (degress, [0, 180])
int distance = 0;    // Distance measured

int second = 0;
int count = 0;

unsigned long time;
void blink_led(byte* payload) {

}

void setup()
{
    init_msg_callbacks();
  subscribe(BLINK_LED, blink_led);
  // Serial output
  Serial.begin(115200);
  
  // Servo control
  myservo.attach(5);
  
  // LIDAR control
  Wire.begin(); // join i2c bus
  //setAcquisition(200);
  stepper.setRPM(10);
  stepper.setMicrostep(16); 
}
void setAcquisition(int val){
  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write(0x02); // sets register pointer to  (0x00)  
  Wire.write(val); // sets register pointer to  (0x00)  
  Wire.endTransmission(); // stop transmitting
  }

// Get a measurement from the LIDAR Lite
int lidarGetRange(void)
{
  int val = -1;
  
  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterMeasure); // sets register pointer to  (0x00)  
  Wire.write((int)MeasureValue); // sets register pointer to  (0x00)  
  Wire.endTransmission(); // stop transmitting

  delay(10); // Wait 20ms for transmit

  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterHighLowB); // sets register pointer to (0x8f)
  Wire.endTransmission(); // stop transmitting

  delay(10); // Wait 20ms for transmit
  
  Wire.requestFrom((int)LIDARLite_ADDRESS, 2); // request 2 bytes from LIDAR-Lite

  if(2 <= Wire.available()) // if two bytes were received
  {
    val = Wire.read(); // receive high byte (overwrites previous reading)
    val = val << 8; // shift high byte to be high 8 bits
    val |= Wire.read(); // receive low byte as lower 8 bits
  }
  
  return val;
}

void serialPrintRange(unsigned long time,int count, int distance)
{
    Serial.print("time: ");
    Serial.print(time);
    Serial.print("\t\tcount: ");
    Serial.print(count);
    Serial.print("\t\tDistance (cm): ");
    Serial.println(distance);
}
void serialPrintRange(double pos, int distance)
{
    Serial.print("position (deg): ");
    Serial.print(pos);
    
    Serial.print("\t\tDistance (cm): ");
    Serial.println(distance);
}
void timeMeasure(){
  distance = lidarGetRange();
    time = millis();
    if(time/1000 >= second + 1){
      second = second + 1;
      count = 0;
      }
      else{
        count = count + 1;
      }
    serialPrintRange(second,count, distance);
    
  }
void loop()
{
  for(int i = 0; i < 200; i++){
    distance = lidarGetRange();
    //serialPrintRange(pos, distance);
    LidarDataMessage msg = LidarDataMessage(distance, pos);
    SendVESCPacket(&msg);
    stepper.move(1);
    pos = pos + 1.8; 
    }
    pos = 0;
}
