/*
 * Function setup()
 * 
 * This fnction is used to setup the Arduino firmware and hardware.
 * DO NOT TOUCH THIS FUCNTION.
 */
void setup()
{
  Serial.begin(9600);
}

/*  
 *  Function mass(long int analogInpt)
 *  Parameters:
 *  -> long int analogInput, the input from the analog weight sensor.
 *  Return:
 *  -> A double representing the mass of an object.
 *  
 *  There are 3 possible functions that can be used, and they all grow at different rates.
 *  Line 28 is a quadratic function.
 *  Line 29 is a quartic function.
 *  Line 30 is a cubic function.
 *  Test any one of these functions, one at a time, to see which function best works with agreed weight sensor setup.
 *  Submit a new issue if these functions do not work. We may have to do some more tesing if these functions do not work.
 */
double mass(long int analogInput)
{
  return 0.0005*(double)analogInput*(double)analogInput + 1.3545*(double)analogInput - 1.7847;
  //return 0.00000007*pow((double)analogInput, 4) - 0.00004*pow((double)analogInput, 3) + 0.0053*(double)analogInput*(double)analogInput + 1.188*(double)analogInput - 0.8405;
  //return 0.000002*pow((double)analogInput, 3) - 0.0005*(double)analogInput*(double)analogInput + 1.4512*(double)analogInput - 3.416;
}

/*
 * Function analogAverage(long int numberArray[])
 * Parameters:
 * -> long int numberArray[], an array containing analog input numbers.
 * Return:
 * -> The central limit average of the analog integer input.
 * 
 * This function calculates the average analog voltage reading number.
 * This function should be kept for testing and development purposes.
 */
long int analogAverage(long int numberArray[])
{
  double sum = 0;
  for(int i=0; i<60; i=i+1)
  {
    sum = sum + (double) numberArray[i];
  }
  return (long int) ((double) sum)/((double) 60);
}

/*
 * Function readVoltageInput()
 * 
 * A function that is used to Find the average binary values of each voltage value for a specific mass.
 * This function should be kept for testing and development purposes.
 */
void readVoltageInput()
{
  long int analogNumbers[60]; // 60 binary value voltage interpretation points.
  for (int i = 0; i<60; i=i+1) // Get 60 data points to calculate binary value interpretation of the voltage.
  {
    int analogNumber = analogRead(A5);
    Serial.print("Analog: ");
    Serial.println(analogNumber);
    analogNumbers[i] = analogNumber;
    delay(1000);
  }
  long int average = analogAverage(analogNumbers);
  Serial.print("Average: "); // Print the average binary value of the voltage input shown.
  Serial.println(average);
  delay(1000);
}

/*
 * Function readMass()
 * 
 * Testing function used read and calculate the mass of an obejct.
 * This function should be kept for testing and development purposes.
 */
void readMass()
{
  // Print the mass of the object at every 1-second interval.
  Serial.println(mass(analogRead(A5)));
  delay(1000);
}

/*
 * Function loop()
 * This is the function that the Arduino executes. It is the entry point of this program.
 * Put any functions that you want to use in here.
 */
void loop()
{
  readVoltageInput();
}
