// Code to read data from moisture sensor, and relay this data to the main computer cluster
// Need some error handling

float buffer[1024];

float dielectric_constant(float R)  {
    float dC = abs(pow(R,4)*(2.589*pow(10,-10))-5.010*pow(10,-7)*pow(R,3)-(3.523*pow(10,-4))*pow(R,2)-(9.135*pow(10,-2))*R+7.457);
    return dC;
}

float moist(float dC)    {
    float moisture = (dC/80) - (0.26/(dC-1)) + 0.59;
    return moisture;
}

void read_moisture(float Raw)    {
        Raw = analogRead(moistsensor);
        dielectricConstant = dielectric_constant(Raw);
        sprintf(buffer, "{\"data\":[\"%e\"]}", moist(dielectricConstant));  
        Publish(buffer);
}
