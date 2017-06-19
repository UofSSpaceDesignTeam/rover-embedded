/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  delay(1000);
  Serial.print("hello");
  
}

float vin(char pin, int R1, int R2){
/*                              //When my C++ code for for loop won't work D;
  int totSensorValue = 0;
  
  for (int x=0; x<20; x++){ 
    int sensorValue = analogRead(pin); 
    delay(2);
    return totSensorValue = totSensorValue + sensorValue;
    }
*/
    int sensorValue1 = analogRead(pin);
    delay(1);

    int sensorValue2 = analogRead(pin);
    delay(1);

    int sensorValue3 = analogRead(pin);
    delay(1);

    int sensorValue4 = analogRead(pin);
    delay(1);

    int sensorValue5 = analogRead(pin);
    delay(1);


    int totSensorValue = sensorValue1 + sensorValue2 + sensorValue3 + sensorValue4 + sensorValue5;
  
  
  float avgSensorValue = totSensorValue/5; //analog read value is averaged to lessen noise

 
 float voltage = avgSensorValue * (3.3 / 8192.0); //convert analog read to voltage
 //Serial.print(voltage);
 Serial.print(" ");
 float actual = (voltage*(R1+R2))/R2; //find voltage input
 return actual;
}


void loop() {

  analogReadResolution(13);
  // read the input on analog pin 8:

  float A_8 = vin( A11, 886000, 114000);
  float A_7 = vin( 23, 870000, 130000);
  float A_6 = vin( 22, 848000, 152000);
  float A_5 = vin( 21, 818000, 182000);
  float A_4 = vin( 20, 773000, 227000);
  float A_3 = vin( 19, 697000, 303000);
  float A_2 = vin( 18, 545000, 455000);
  float A_1 = vin( 17, 90900,909000);
  char buff[256];
  sprintf(buff, "2-1 %0.2f    3-2 %0.2f     4-3 %0.2f     5-4 %0.2f     6-5 %0.2f     7-6 %0.2f    8-7 %0.2f", A_2-A_1, A_3-A_2,  A_4-A_3, A_5-A_4, A_6-A_5, A_7 - A_6, A_8 - A_7);

  Serial.println( buff); //A_6-A_5, A_5-A_4, A_4-A_3, A_3-A_2, A_2-A_1);

  //analog read, then delay for 10, repeat 5 times for every single thing, do analog read within function vin

  delay(100);        // delay in between reads for stability
}
