#include "DACx311.h"
#include <SPI.h>

#if defined (SPI_HAS_TRANSACTION)
  static SPISettings mySPISettings;
#elif defined (__AVR__)
  static uint8_t SPCRbackup;
  static uint8_t mySPCR;
#endif

DACxx11::DACxx11(){}

void DACxx11::begin( byte csPin, byte chip, float maxVoltage )
{
  _cs = csPin;
  _chip = chip;
  _maxVoltage = maxVoltage;

  if( _chip == DAC5311 )
  {
    bits_to_write = 8;
    bit_shift = 6;
  } else if( _chip == DAC6311 )
  {
    bits_to_write = 10;
    bit_shift = 4;
  } else if( _chip == DAC7311 )
  {
    bits_to_write = 12;
    bit_shift = 2;
  } else {
    
  }
  
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
  csport    = portOutputRegister(digitalPinToPort(_cs));
  cspinmask = digitalPinToBitMask(_cs);

#if defined (SPI_HAS_TRANSACTION)
  SPI.begin();
  mySPISettings = SPISettings(8000000, MSBFIRST, SPI_MODE0);
#elif defined (__AVR__)
  SPCRbackup = SPCR;
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);
  mySPCR = SPCR; // save our preferred state
  SPCR = SPCRbackup;  // then restore
#elif defined (__SAM3X8E__)
  SPI.begin();
  SPI.setClockDivider(21); //4MHz
  SPI.setDataMode(SPI_MODE0);
#endif

  _singleVoltageStep = float(5.0) / pow( 2, bits_to_write );
}

void DACxx11::setVoltage( float voltage )
{
  // keep within limits
  if( voltage > _maxVoltage ){ voltage = _maxVoltage; }
  else if( voltage < 0 ){ voltage = 0; }
  
  // calc and set voltage
  uint16_t value = int(voltage/_singleVoltageStep);
  value = value << bit_shift;
  uint8_t upper = value >> 8;
  uint8_t lower = value & 0xff;
  writedata(upper,lower);
}

inline void DACxx11::writedata(uint8_t upper, uint8_t lower)
{
  *csport &= ~cspinmask;
  
  #if defined (SPI_HAS_TRANSACTION)
    SPI.beginTransaction(mySPISettings);
#endif

#if defined (SPI_HAS_TRANSACTION)
  SPI.transfer(upper);
  SPI.transfer(lower);
#elif defined (__AVR__)
  SPCRbackup = SPCR;
  SPCR = mySPCR;
  SPI.transfer(upper);
  SPI.transfer(lower);
  SPCR = SPCRbackup;
#elif defined (__arm__)
  SPI.setClockDivider(21); //4MHz
  SPI.setDataMode(SPI_MODE0);
  SPI.transfer(upper);
  SPI.transfer(lower);
#endif
  
#if defined (SPI_HAS_TRANSACTION)
  SPI.endTransaction();
#endif
  
  *csport |= cspinmask;
}
