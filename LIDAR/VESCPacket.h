#ifndef VESCPACKET_H_
#define VESCPACKET_H_
/*
 * The following are taken from Benjamin Vedder's bldc firmware:
 * https://github.com/vedderb/bldc/blob/master/buffer.c
 */
void buffer_append_int16(uint8_t* buffer, int16_t number, int32_t *index);
void buffer_append_uint16(uint8_t* buffer, uint16_t number, int32_t *index);
void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index);
void buffer_append_uint32(uint8_t* buffer, uint32_t number, int32_t *index);
void buffer_append_float16(uint8_t* buffer, float number, float scale, int32_t *index);
void buffer_append_float32(uint8_t* buffer, float number, float scale, int32_t *index);
int16_t buffer_get_int16(const uint8_t *buffer, int32_t *index);
uint16_t buffer_get_uint16(const uint8_t *buffer, int32_t *index);
int32_t buffer_get_int32(const uint8_t *buffer, int32_t *index);
uint32_t buffer_get_uint32(const uint8_t *buffer, int32_t *index);
float buffer_get_float16(const uint8_t *buffer, float scale, int32_t *index);
float buffer_get_float32(const uint8_t *buffer, float scale, int32_t *index);
bool buffer_get_bool(const uint8_t *buffer, int32_t *index);
void buffer_append_bool(uint8_t *buffer,bool value, int32_t *index);

/* Our functions begin: */

#define NR_MSGS 43

extern void (*msg_callbacks[NR_MSGS + 1])(byte *payload);
extern char *msg_names[NR_MSGS];

/*
 * Message types.
 */
typedef enum {
  REQ_SUBSCRIPTION = 36,
  EXAMPLE_SEND = 39,
  BLINK_LED = 40,
  LIDAR_DATA = 41,
  COMPASS_DATA = 42,
  BATT_READ = 43
} message_t;


class VESCMessage {
public:
    int id;
    int length;
    VESCMessage(){
        //id = 0;
        length = 0;
    }
    virtual byte *encode() =0;
};

/*
 * Initialize all the callbacks in the look up table to NULL.
 */
void init_msg_callbacks(void);

/*
 * Sends the message(s?) this device is currently subscribed to.
 */
void sendSubscriptions(byte *payload);

/*
 * Subscribe to a message by message id.
 * Takes in the message id of the VESC message, and
 * A call back function to call when that message is received
 */
void subscribe(int msg_id, void (*callback)(byte *payload));

/*
 * Calculates the check sum of a message contained in buf of length len
 */
unsigned short crc16(unsigned char *buf, unsigned int len);

/*
 * Sends a message payload of lenght lenPay to another VESC device over serial.
 * Returns the number of bytes that were sent.
 */
int SendVESCPacket(VESCMessage *msg);

/*
 * Takes a message recieved over serial, and extracts it to a usable payload.
 * If the checksum works out, it returns true. If the checksum fails, it returns false.
 */
bool UnpackMessage(uint8_t* message, int lenMes, uint8_t* payload, int* lenPay);

/*
 * Reads a packet from another VESC device over serial.
 * The packet is coppied into buffer.
 */
int ReadVESCPacket(byte* buffer, int max_len);


class SubscribeMessage : public VESCMessage {
public:
    int id = 36;
    char *subscription;
    SubscribeMessage(char *sub);
    byte *encode();
};

class ExampleSendMessage : public VESCMessage {
public:
    int id = 39;
    char str[256];
    ExampleSendMessage(char *str);
    byte *encode();
};

/*
 * You can define a class to contain and parse the values
 * of the VESC Message you are using.
 * These classes should only be used for packing and unpacking
 * bytes, everythin else should be done in another function or class.
 */
class BlinkMessage : public VESCMessage{
public:
    int id = 40;
    int value; // should just be 0 or 1
    BlinkMessage(byte *payload);
    byte *encode() {return NULL;}
};


class LidarDataMessage : public VESCMessage {
public:
  int id = LIDAR_DATA;
  int32_t distance;
  float angle;
  int32_t tilt;
  LidarDataMessage(int dist, float ang, int tilt);
  byte *encode();
};

class CompassDataMessage : public VESCMessage {
public:
  int id = COMPASS_DATA;
  float heading3;
  float pitch3;
  float roll3;
  CompassDataMessage(float heading2, float pitch2, float roll2);
  byte *encode();
};

class BatteryVoltageRead : public VESCMessage {
public:
	int id = BATT_READ;
	float V_1, V_2, V_3, V_4, V_5, V_6, V_7, V_8;
	BatteryVoltageRead(float V1, float V2, float V3, float V4, float V5, float V6, float V7, float V8);
	byte *encode();

};



#endif
