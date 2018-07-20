#ifndef ROBOCLUSTER
#define ROBOCLUSTER
#include <stdarg.h>

#define BUFF_SIZE 512
#define MAX_CALLBACKS 8

typedef void (*callback)(char*);

char *g_messages[MAX_CALLBACKS];
callback g_callbacks[MAX_CALLBACKS];
int g_num_messages = 0;

void dummy_handler(char*) {
    // This does nothing, but you can define your own
    // function and enable it with set_message_handler(yourFunction).
}

char *get_key(char* buff, int len) {
    int start, end = 0;
    int k = 0;
    char key[BUFF_SIZE];
    memset(key, 0, BUFF_SIZE);
    for(int i=0; i<len; i++) {
        if (end != 0) {
            break;
        }
        switch(buff[i]) {
            case '{':
                start = i;
                break;
            case ':':
                end = i;
                break;
            case '"':
                break;
            default:
                key[k] = buff[i];
                k++;
        }
    }
    return key;
}

char *get_data(char* buff, int len) {
    int start = 0;
    int end = 0;
    int k = 0;
    char data[BUFF_SIZE];
    memset(data, 0, BUFF_SIZE);
    for(int i=0; i<len; i++) {
        if (end != 0) {
            break;
        }
        switch(buff[i]) {
            case ':':
                start = i+1;
                i++; // skip the space
                break;
            case '{':
                break;
            case '}':
                end = i;
                break;
            case '"':
                break;
            default:
                if (start != 0) {
                    data[k] = buff[i];
                    k++;
                }
        }
    }
    return data;

}

void set_messages(int n_args, ...) {
    va_list messages;
    va_start(messages, n_args);
    for(int i=0; i<n_args; i++) {
        g_messages[i] = va_arg(messages, char*);
    }
    va_end(messages);
    g_num_messages = n_args;
}

void set_callbacks(int n_args, ...) {
    va_list callbacks;
    va_start(callbacks, n_args);
    for(int i=0; i<n_args; i++) {
        g_callbacks[i] = va_arg(callbacks, callback);
    }
    va_end(callbacks);

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
        char *key = get_key(buff, b[0]);
        char *data = get_data(buff, b[0]);
        for(int i=0; i<g_num_messages; i++) {
            if (strcmp(key, g_messages[i]) == 0)
                digitalWrite(13, HIGH);
                g_callbacks[i](data);
        }
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
