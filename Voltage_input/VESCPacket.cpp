#include <Arduino.h>

#include "VESCPacket.h"

void (*msg_callbacks[NR_MSGS+1])(byte *payload);
char *msg_names[NR_MSGS];
char *g_subscriptions;
void sendSubscriptions(byte *payload) {
  SubscribeMessage sub = SubscribeMessage(g_subscriptions);
  Serial1.println(sub.subscription); // Hacks; won't work with out this for some reason...
  SendVESCPacket(&sub);
}

void init_msg_callbacks(void) {
  for(int i=0; i<=NR_MSGS; i++) {
    msg_callbacks[i] = NULL;
  }
  msg_callbacks[REQ_SUBSCRIPTION] = sendSubscriptions;
  /* Add new message strings/keys here */
  msg_names[EXAMPLE_SEND] = "examplesend";
  msg_names[BLINK_LED] = "blink";
  msg_names[LIDAR_DATA] = "lidar_data";
  msg_names[ACCELEROMETER_DATA] = "accelerometer_data";
}

void subscribe(int msg_id, void (*callback)(byte *payload)) {
  if(msg_id >= 0 && msg_id <= NR_MSGS) {
    msg_callbacks[msg_id] = callback;
    g_subscriptions = msg_names[msg_id];
  }
}

const unsigned short crc16_tab[] = { 0x0000, 0x1021, 0x2042, 0x3063, 0x4084,
    0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad,
    0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7,
    0x62d6, 0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a,
    0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672,
    0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719,
    0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7,
    0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948,
    0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50,
    0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b,
    0xab1a, 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97,
    0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe,
    0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca,
    0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2, 0x20e3,
    0x5004, 0x4025, 0x7046, 0x6067, 0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d,
    0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214,
    0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c,
    0xc50d, 0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3,
    0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d,
    0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806,
    0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e,
    0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1,
    0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b,
    0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0,
    0x0cc1, 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0 };


unsigned short crc16(unsigned char *buf, unsigned int len) {
  unsigned int i;
  unsigned short cksum = 0;
  for (i = 0; i < len; i++) {
    cksum = crc16_tab[(((cksum >> 8) ^ *buf++) & 0xFF)] ^ (cksum << 8);
  }
  return cksum;
}

int SendVESCPacket(VESCMessage *msg){
  uint16_t crcPayload;
  int lenPacket;

  int lenPay = msg->length + 1; // ID + message
  uint8_t *payload = msg->encode();
  int packet_idx = 0; 
  uint8_t packet[lenPay+6]; // header(2 | 3) + 1-2 byte length + 2 byte crc + footer(3) + "\0"


  // create payload first
  crcPayload = crc16(payload, lenPay);

  //Create packet from payload
  if (lenPay <= 256)
  {
    lenPacket = lenPay+5;
    packet[packet_idx++] = 2;
    packet[packet_idx++] = lenPay;
  }
  else
  {
    lenPacket = lenPay+6;
    packet[packet_idx++] = 3;
    packet[packet_idx++] = (uint8_t)(lenPay >> 8);
    packet[packet_idx++] = (uint8_t)(lenPay & 0xFF);
  }
  memcpy(&packet[packet_idx], payload, lenPay);

  packet_idx += lenPay;
  packet[packet_idx++] = (uint8_t)(crcPayload >> 8);
  packet[packet_idx++] = (uint8_t)(crcPayload & 0xFF);
  packet[packet_idx++] = 3;
  //Sending package
  for(int i=0; i<lenPacket; i++) {
    Serial.print((char)packet[i]);
  }
  Serial.print("\n");
  free(payload); // this was allocated in msg.encode()

  //Returns number of send bytes
  return packet_idx;
}

bool UnpackMessage(uint8_t* message, int lenMes, uint8_t* payload, int* lenPay) {
  uint16_t crcMessage = 0;
  uint16_t crcPayload = 0;
  *lenPay = message[1];
  crcMessage = message[lenMes - 3] << 8;
  crcMessage &= 0xFF00;
  crcMessage |= (message[lenMes - 2]) & 0x00FF;
  //Extract payload:
  memcpy(payload, &message[2], message[1]);

  crcPayload = crc16(payload, message[1]);

  return crcPayload == crcMessage;
}

int ReadVESCPacket(byte* buffer, int max_len){
  int count = 0;
  int len = 0;
  
  buffer[count++] = Serial.read();
  if(buffer[0] == 2) { // single byte length field
    buffer[count++] = Serial.read();
    len = buffer[1];
  } else if(buffer[0] == 3) { // two byte length field
    buffer[count++] = Serial.read();
    buffer[count++] = Serial.read();
    len = (buffer[1] << 8) & 0xFF00;
    len |= buffer[2] & 0x00FF;
  } else {
    return -1; // invalid header
  }
  
  if(len > max_len) {
    return -2; // buffer not long enough to fit message
  }
  
  for(int i = 0; i< len; i++) // read in each byte of the message
    buffer[count++] = Serial.read();
    
  buffer[count++] = Serial.read(); // checksum byte 1
  buffer[count++] = Serial.read(); // checksum byte 2
  buffer[count] = Serial.read(); // footer (3)
  if(buffer[count] != 3) {
    return -3; // bad footer
  }
  return count;
}

/* 
 *  serialEvent is called whenever data is send to the arduino over serial.
 *  https://www.arduino.cc/en/Reference/SerialEvent
 */
void serialEvent() {
  byte inByte[256];
  byte payload[256];
  int lenPay = 0;
  int bytes_read;
  noInterrupts(); // make sure we can only handle one message at a time.
  if(Serial.available()) {
      // read and parse vesc message
      bytes_read = ReadVESCPacket(inByte, 256);
      UnpackMessage(inByte, bytes_read, payload, &lenPay);

      // make sure it contains a valid message id
      if(payload[0] >= 0 && payload[0] <= NR_MSGS) {
        // If a callback was registered, call it
        if(msg_callbacks[payload[0]] != NULL) {
          msg_callbacks[payload[0]](payload);
        }
      }
  }
  interrupts(); // re-enable interrupts
}

/*
 * The encode method is for converting your message object
 * to a binary representation.
 */
byte *SubscribeMessage::encode() {
    byte *payload = (byte *)malloc(length + 1);
    payload[0] = id;
    memcpy(payload+1, subscription, length);
    return payload;
}

/*
 * You can write constructors to create messages
 * based on the data you want to initialize the fields with.
 */
SubscribeMessage::SubscribeMessage(char *sub) {
  strcpy(this->subscription, sub);
  length = strlen(sub);
}

/*
 * Define your message constructor to parse the
 * payload byte array to populate the appropriate
 * values in the class
 */
BlinkMessage::BlinkMessage(byte *payload){
        int32_t index = 1;
        value = buffer_get_int32(payload, &index);
        length = sizeof(int);
}


byte *ExampleSendMessage::encode() {
    byte *payload = (byte *)malloc(length + 1);
    payload[0] = id;
    memcpy(payload+1, str, length);
    return payload;
}

ExampleSendMessage::ExampleSendMessage(char *str) {
    strcpy(this->str, str);
    length = strlen(str);
}

LidarDataMessage::LidarDataMessage(int dist, float ang, int tilt_, int finished_) {
  distance = dist;
  angle = ang;
  tilt = tilt_;
  finished = finished_;
  length = 3*sizeof(int) + sizeof(float);
}

byte *LidarDataMessage::encode() {
    byte *payload = (byte *)malloc(length + 1);
    payload[0] = id;
    int32_t index = 1;
    buffer_append_int32(payload, distance, &index);
    buffer_append_float32(payload, angle, 100, &index);
    buffer_append_int32(payload, tilt, &index);
    buffer_append_int32(payload, finished, &index);
    return payload;
}

CompassDataMessage::CompassDataMessage(float heading2, float pitch2, float roll2) {
  heading3 = heading2;
  pitch3 = pitch2;
  roll3 = roll2;
  length = 3*sizeof(float);
}

byte *CompassDataMessage::encode() {
    byte *payload = (byte *)malloc(length + 1);
    payload[0] = COMPASS_DATA;
    int32_t index = 1;
    buffer_append_float32(payload, heading3, 1000, &index);
    buffer_append_float32(payload, pitch3, 1000, &index);
    buffer_append_float32(payload, roll3, 1000, &index);
  return payload;
}

AccelerometerDataMessage::AccelerometerDataMessage(float x_, float y_, float z_) {
  x = x_;
  y = y_;
  z = z_;
  length = 3*sizeof(float);
}

byte *AccelerometerDataMessage::encode() {
  byte *payload = (byte*) malloc(length+1);
  payload[0] = ACCELEROMETER_DATA;
  int32_t index = 1;
  buffer_append_float32(payload, x, 1000, &index);
  buffer_append_float32(payload, y, 1000, &index);
  buffer_append_float32(payload, z, 1000, &index);
  return payload;
}

/*
 * The following are taken from Benjamin Vedder's bldc firmware:
 * https://github.com/vedderb/bldc/blob/master/buffer.c
 */
void buffer_append_int16(uint8_t* buffer, int16_t number, int32_t *index) {
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void buffer_append_uint16(uint8_t* buffer, uint16_t number, int32_t *index) {
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {
    buffer[(*index)++] = number >> 24;
    buffer[(*index)++] = number >> 16;
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void buffer_append_uint32(uint8_t* buffer, uint32_t number, int32_t *index) {
    buffer[(*index)++] = number >> 24;
    buffer[(*index)++] = number >> 16;
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void buffer_append_float16(uint8_t* buffer, float number, float scale, int32_t *index) {
    buffer_append_int16(buffer, (int16_t)(number * scale), index);
}

void buffer_append_float32(uint8_t* buffer, float number, float scale, int32_t *index) {
    buffer_append_int32(buffer, (int32_t)(number * scale), index);
}

int16_t buffer_get_int16(const uint8_t *buffer, int32_t *index) {
    int16_t res =	((uint16_t) buffer[*index]) << 8 |
        ((uint16_t) buffer[*index + 1]);
    *index += 2;
    return res;
}

uint16_t buffer_get_uint16(const uint8_t *buffer, int32_t *index) {
    uint16_t res = 	((uint16_t) buffer[*index]) << 8 |
        ((uint16_t) buffer[*index + 1]);
    *index += 2;
    return res;
}

int32_t buffer_get_int32(const uint8_t *buffer, int32_t *index) {
    int32_t res =	((uint32_t) buffer[*index]) << 24 |
        ((uint32_t) buffer[*index + 1]) << 16 |
        ((uint32_t) buffer[*index + 2]) << 8 |
        ((uint32_t) buffer[*index + 3]);
    *index += 4;
    return res;
}

uint32_t buffer_get_uint32(const uint8_t *buffer, int32_t *index) {
    uint32_t res =	((uint32_t) buffer[*index]) << 24 |
        ((uint32_t) buffer[*index + 1]) << 16 |
        ((uint32_t) buffer[*index + 2]) << 8 |
        ((uint32_t) buffer[*index + 3]);
    *index += 4;
    return res;
}

float buffer_get_float16(const uint8_t *buffer, float scale, int32_t *index) {
    return (float)buffer_get_int16(buffer, index) / scale;
}

float buffer_get_float32(const uint8_t *buffer, float scale, int32_t *index) {
    return (float)buffer_get_int32(buffer, index) / scale;
}
