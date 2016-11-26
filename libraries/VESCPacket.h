#ifndef VESCPACKET_H_
#define VESCPACKET_H_

unsigned short crc16(unsigned char *buf, unsigned int len);
int SendVESCPacket(uint8_t* payload, int lenPay);
bool UnpackPayload(uint8_t* message, int lenMes, uint8_t* payload, int* lenPay);
int ReadVESCMessage(byte* buffer);

#endif