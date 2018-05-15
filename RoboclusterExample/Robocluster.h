
char *DEVICE_NAME = "TestName";

void serialEvent() {
    digitalWrite(13, HIGH);
    char buff[1024];
    sprintf(buff, "{\"name\":\"%s\"}", DEVICE_NAME);
    char b[10];
    Serial.readBytes(b, 7);
    Serial.write(strlen(buff));
    Serial.print(buff);
    digitalWrite(13, LOW);
}

void set_name(char* s) {
    DEVICE_NAME = s;
}

void Publish(char *message) {
    int len = strlen(message);
    Serial.write(len);
    Serial.print(message);
}
