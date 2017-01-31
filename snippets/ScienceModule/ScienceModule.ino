int DIN = 2; //DIN set to pin 2
int CLK = 3; //CLK set to pin 3
int LE = 4; //LE set to pin 4
int OE_bar = 5; //Operation enable set to pin 5

int result; //range from 10-880

void printTime(){
  Serial.print(millis()/60000);   
  Serial.print(" min ");
  Serial.print((millis()/1000)%60);
  Serial.print(".");
  if ( ((millis()/10)%100) < 10) Serial.print("0");
  Serial.print((millis()/10)%100);   
  Serial.print(" sec: ");
}


void readData() {
  delay(100);  
  printTime();
  Serial.println(analogRead(A0), DEC);
  delay(100);  
  printTime();
  Serial.println(analogRead(A0), DEC);
  delay(100);  
  printTime();
  Serial.println(analogRead(A0), DEC);
  delay(100);  
  printTime();
  Serial.println(analogRead(A0), DEC);
  delay(100);  
  printTime();
  Serial.println(analogRead(A0), DEC);
}


void setup() {
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(LE, OUTPUT);
  
  digitalWrite(DIN, 0);
  digitalWrite(CLK, 0);
  digitalWrite(LE, 0);
  digitalWrite(OE_bar, 0);
  
  Serial.begin(57600);
}

void loop() {
  digitalWrite(LE, 1);
  readData();
  digitalWrite(CLK, 1);
  readData();
  digitalWrite(CLK, 0);
  readData();
  
  //First Pulse
  digitalWrite(DIN, 0);
  readData();
  digitalWrite(CLK, 1);
  readData();
  digitalWrite(CLK, 0);
  readData();

  //Second Pulse
  digitalWrite(DIN, 0);
  readData();
  digitalWrite(CLK, 1);
  readData();
  digitalWrite(CLK, 0);
  readData();

   //Third Pulse
  digitalWrite(DIN, 1);
  readData();
  digitalWrite(CLK, 1);
  readData();
  digitalWrite(CLK, 0);
  readData();

   //Fourth Pulse
  digitalWrite(DIN, 1);
  readData();
  digitalWrite(CLK, 1);
  readData();
  digitalWrite(CLK, 0);
  readData();

   //Fifth Pulse
  digitalWrite(DIN, 1);
  readData();
  digitalWrite(CLK, 1);
  readData();
  digitalWrite(CLK, 0);
  readData();

  //Sixth Pulse
  digitalWrite(DIN, 1);
  readData();
  digitalWrite(CLK, 1);
  readData();
  digitalWrite(CLK, 0);
  readData();

  //Seventh Pulse
  digitalWrite(DIN, 1);
  readData();
  digitalWrite(CLK, 1);
  readData();
  digitalWrite(CLK, 0);
  readData();

  //Last Pulse
  digitalWrite(DIN, 1);
  readData();
  digitalWrite(CLK, 1);
  readData();
  digitalWrite(CLK, 0);
  readData();
  
  digitalWrite(LE, 0);
  readData();
  digitalWrite(CLK, 1);
  readData();
  digitalWrite(CLK, 0);
  readData();
}


