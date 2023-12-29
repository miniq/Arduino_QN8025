#include <Wire.h>
#include "QN8025.h"

#ifndef __DEBUG__
#define __DEBUG__ 1
#endif

#define INITIAL_FREQUENCY 92.9
#define INITIAL_VOLUME 0x00

unsigned int frequency = (unsigned int)(100 * (INITIAL_FREQUENCY));  // MHz

byte volume = INITIAL_VOLUME;


void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial)
    ;  // Leonardo: wait for Serial Monitor
  if (__DEBUG__) Serial.println("\n\n---==| I2C QN8025 |==---\n\n");

  QN8025_Write(SYSTEM1, 0x19);  // CCA_CH_DIS = 1, RDSEN =1, RXREQ =1
  QN8025_Write(PLL_DIV2, 0x20);

  if (__DEBUG__) {
    Serial.print("Setting default volume: ");
    Serial.println(volume);
  }
  set_volume(volume);

  if (__DEBUG__) {
    Serial.print("Setting default frequency ");
    Serial.print(frequency / 100);
    Serial.print(" (channel: ");
    Serial.print(freq2channel(frequency));
    Serial.println(")");
  }
  set_freq(frequency);

  if (__DEBUG__) {
    Serial.print("Readed channel: ");
    Serial.println(read_channel());
  }
}

// Defined commands
const int CMD_SET_FREQUENCY = 0x01;
const int CMD_SET_VOLUME = 0x02;
const int CMD_MUTE = 0x04;

// Other constants
const int CHAR_NEW_LINE = 0x0a;

int muted = 0; 

void printx(int character) {
  // Serial.print("Character: ");
  // Serial.print(character);
  // Serial.print(" | ");
  // Serial.println(character - '0');
}

void loop() {
  int command = 0;
  int parameter = 0;
   
  int available = Serial.available();
  if (__DEBUG__) {
    if (available > 0) {
      Serial.print("Available character(s): ");
      Serial.println(available);
    }
  }
  while (Serial.available() > 0) {
    int character = Serial.read();
    delay(5);  // needed for correct read from serial, otherwise character has incorrect value

    if (character != CHAR_NEW_LINE) {  // character is not \n
      if (0 == command) {
        // determine command
        if (__DEBUG__) Serial.println("Parsing command");
        switch (character) {
          case 'F':
            command = CMD_SET_FREQUENCY;
            if (__DEBUG__) Serial.println("Command F: ");
            break;

          case 'V':
            command = CMD_SET_VOLUME;
            if (__DEBUG__) Serial.println("Command V: ");
            break;

          case 'M':
            command = CMD_MUTE;
            if (__DEBUG__) Serial.println("Command M: ");
            break;

          default:
            if (__DEBUG__) Serial.println("Unknown command");
            command = 0;
        }
        if (__DEBUG__) Serial.println("End parsing command");
      } else {
        if (__DEBUG__) Serial.println("Parsing parameter");
        // determina command parameter
        parameter *= 10;
        switch (command) {
          case CMD_SET_FREQUENCY:
            if (__DEBUG__) {
              printx(character);
            }
            parameter += character - '0';
            break;

          case CMD_SET_VOLUME:
            if (__DEBUG__) {
              printx(character);
            }
            parameter += character - '0';
            break;

          case CMD_MUTE:
            if (__DEBUG__) {
              printx(character);
            }
            parameter += character - '0';
            break;

          default:
            if (__DEBUG__) Serial.println("parameter not provided");
        }
      }
    } else {
      if (__DEBUG__) Serial.println("End parsing parameter");
      // End of command and parameter
      if (__DEBUG__) {
        Serial.print("Command: ");
        Serial.print(command);
        Serial.print("\t");
        Serial.print("Parameter: ");
        Serial.print(parameter, DEC);
        Serial.println();
      }
      switch (command) {
        case CMD_SET_FREQUENCY:
          // set frequency
          frequency = parameter;
          set_freq(frequency);
          break;

        case CMD_SET_VOLUME:
          // set volume
          volume = (byte)parameter;
          set_volume(volume);
          break;

        case CMD_MUTE:
          // mute/unmute
          if ( !muted ) {
            muted = 1;
            set_volume(-1);
          } else {
            muted = 0;
            set_volume(volume);
          }
          break;

        default:
          if (__DEBUG__) Serial.println("Unexpected Unknown command");
      }

      command = 0;
      parameter = 0;
    }
  }
}