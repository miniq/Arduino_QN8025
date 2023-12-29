
#include <Wire.h>
#include "QN8025.h"

unsigned int freq2channel(unsigned int freq) {
  return (unsigned int)((freq - 6175) / 5);
}

unsigned int channel2freq(unsigned int ch) {
  return (unsigned int)(6175 + 5 * ch);
}

void set_channel(unsigned int ch) {
  QN8025_Write(CH, (ch & 0xff));
  QN8025_Write(CH_STEP, ch >> 8);
}

void set_freq(unsigned int freq) {
  unsigned int ch;

  ch = freq2channel(freq);
  set_channel(ch);
}

unsigned int read_channel(void) {
  byte ch, ch_s;

  Wire.beginTransmission(QN8025_ADDRESS);
  Wire.write(CH);
  Wire.endTransmission();
  Wire.requestFrom(QN8025_ADDRESS, 1);
  while (Wire.available()) {
    ch = Wire.read();
  }
  Wire.beginTransmission(QN8025_ADDRESS);
  Wire.write(CH_STEP);
  Wire.endTransmission();
  Wire.requestFrom(QN8025_ADDRESS, 1);
  while (Wire.available()) {
    ch_s = Wire.read();
  }

  return ((ch_s & 0x03) << 8) | ch;
}

void QN8025_Write(byte adress, byte data) {
  Wire.beginTransmission(QN8025_ADDRESS);
  Wire.write(adress);
  Wire.write(data);
  Wire.endTransmission();
}

void set_volume(byte volume) {
  QN8025_Write(VOL_CTL, (volume | 0x40));
}