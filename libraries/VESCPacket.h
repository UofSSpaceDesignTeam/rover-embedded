#ifndef VESCPACKET_H_
#define VESCPACKET_H_

/*
 * Message types.
 */
typedef enum {
  REQ_SUBSCRIPTION = 36,
  BLINK_LED = 40
} message_t;

/*
 * Calculates the check sum of a message contained in buf of length len
 */
unsigned short crc16(unsigned char *buf, unsigned int len);

/*
 * Sends a message payload of lenght lenPay to another VESC device over serial.
 * Returns the number of bytes that were sent.
 */
int SendVESCPacket(int msgID, void* value, int lenValue);

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

#endif
