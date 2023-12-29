#ifndef QN8025_h
#define QN8025_h

#include <Arduino.h>

#define QN8025_ADDRESS 0x10

#define SYSTEM1 0x00
#define STATUS1 0x04
#define CH 0x07
#define CH_START 0x08
#define CH_STOP 0x09
#define CH_STEP 0x0A
#define VOL_CTL 0x14
#define CCA 0x01
#define RDSD0 0x0Bf
#define RDSD1 0x0C
#define RDSD2 0x0D
#define RDSD3 0x0E
#define RDSD4 0x0F
#define RDSD5 0x10
#define RDSD6 0x11
#define RDSD7 0x12
#define PLL_DIV2 0x17
#define STATUS2 0x13
#define RDS_RXTXUPD 0x07
#define RDSSYNC 0x04
#define RDS0ERR 0x03
#define RDS1ERR 0x02
#define RDS2ERR 0x01
#define RDS3ERR 0x00

unsigned int freq2channel(unsigned int freq);
unsigned int channel2freq(unsigned int ch);
void set_channel(unsigned int ch);
void set_freq(unsigned int freq);
unsigned int read_channel(void);
void QN8025_Write(byte adress, byte data);
void set_volume(byte volume);

#endif

