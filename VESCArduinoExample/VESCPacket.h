#ifndef VESCPACKET_H_
#define VESCPACKET_H_

/*
 * Calculates the check sum of a message contained in buf of length len
 */
unsigned short crc16(unsigned char *buf, unsigned int len);

/*
 * Sends a message payload of lenght lenPay to another VESC device over serial.
 * Returns the number of bytes that were sent.
 */
int SendVESCPacket(uint8_t* key, int lenKey, uint8_t* value, int lenValue);

/*
 * Takes a message recieved over serial, and extracts it to a usable payload.
 * If the checksum works out, it returns true. If the checksum fails, it returns false.
 */
bool UnpackMessage(uint8_t* message, int lenMes, uint8_t* payload, int* lenPay);

/*
 * Reads a message from another VESC device over serial.
 * The message is coppied into buffer.
 */
int ReadVESCMessage(byte* buffer);

#endif
