#ifndef ROBOCLUSTER
#define ROBOCLUSTER

#define BUFF_SIZE 512

void dummy_handler(char*) {
    // This does nothing, but you can define your own
    // function and enable it with set_message_handler(yourFunction).
}

char *DEVICE_NAME = "TestName";
void (*g_message_handler)(char*) = dummy_handler;


void serialEvent() {
    //digitalWrite(13, HIGH);
    char buff[BUFF_SIZE];
    memset(buff, 0, BUFF_SIZE);
    char b[10];
    b[0] = Serial.read();
    if (b[0] == 0x02 || b[0] == 0x03) {
        Serial.readBytes(b, 6);
        sprintf(buff, "{\"name\":\"%s\"}", DEVICE_NAME);
        Serial.write(strlen(buff));
        Serial.print(buff);
    } else {
        Serial.readBytes(buff, b[0]);
        g_message_handler(buff);
    }
    //digitalWrite(13, LOW);
}

void set_name(char* s) {
    DEVICE_NAME = s;
}

void set_message_handler(void (*func)(char*)) {
    g_message_handler = func;
}

void Publish(char *message) {
    int len = strlen(message);
    Serial.write(len);
    Serial.print(message);
}

void s_delay(int value) {
    if (Serial.available()) {
        serialEvent();
    }
    delay(value);

}

#endif
