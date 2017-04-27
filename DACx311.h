/***************************************************
  This library provides simple usage of the DACxx11 chips
  from TI. These are single- channel, voltage output
  digital-to-analog converter and great for setting a
  voltage reference. DAC5311, DAC3311 and DAC7311 are
  supported, but only DAC7311 is tested.

  Jens Chr Brynildsen April 2017 (MIT license)
  
  Multi-MCU SPI code for arm & AVR was borrowed
  from Adafruit's excellent ST7735 lib:
  https://github.com/adafruit/Adafruit-ST7735-Library
  
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit! http://www.adafruit.com

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#ifndef DACX311_H
#define DACX311_H

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#if defined(__SAM3X8E__)
  #include <include/pio.h>
  #define PROGMEM
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
  #define pgm_read_word(addr) (*(const unsigned short *)(addr))
  typedef unsigned char prog_uchar;
#elif defined(__AVR__)
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// Select one of the IC's from below (sets the resolution)
#define DAC5311 1
#define DAC6311 2
#define DAC7311 3

class DACxx11 {
  public:
    DACxx11();
    void begin( byte csPin, byte chip, float maxVoltage = 3.3 );
    void setVoltage( float c );
  private:
    byte _chip;
    void writedata(uint8_t upper, uint8_t lower);
    uint8_t bits_to_write;
    uint8_t bit_shift;
    float _singleVoltageStep;
    float _maxVoltage;

    #if defined(__AVR__) || defined(CORE_TEENSY)
    volatile uint8_t *csport;
    uint8_t  _cs, cspinmask;
  #elif defined(__arm__)
    volatile RwReg  *csport;
    uint32_t  _cs, cspinmask;
    int32_t   _rst;  // Must use signed type since a -1 sentinel is assigned.
  #endif
};

#endif /* DACX311_H */

