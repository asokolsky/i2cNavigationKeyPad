/********************************************************\
 *                                                      *
 * Simple i2c navigation keypad designed to support     *
 * multiple simultaneous key presses.                   *
 * e.g. Arrows up and left                              *
 *                                                      *
 * Alex Sokolsky                                        *
 *   v0.1  11/17/2016                                   *
 *                                                      *
\********************************************************/


#ifndef i2cNavigationKeyPad_H
#define i2cNavigationKeyPad_H

/**
 * Possible addresses (from PCF8574 datasheet)
 * 
A2 A1 A0  I2C Addr
         Read Write
 0  0  0 0x41 0x40
 0  0  1 0x43 0x42
 0  1  0 0x45 0x44
 0  1  1 0x47 0x46
 1  0  0 0x49 0x48
 1  0  1 0x4B 0x4A
 1  1  0 0x4D 0x4C
 1  1  1 0x4F 0x4E
 */

class i2cNavigationKeyPad
{

public:
  i2cNavigationKeyPad(byte addr) : m_address(addr)
  {
    
  }

  void setup();

  bool readAndDispatch();

protected:
  uint8_t read8();
  void write8(uint8_t value); 

  int16_t getLastError() 
  {
    int16_t err = m_error;
    m_error = 0;
    return err;
  }

  /** I2C read address where keypad is located */
  uint8_t m_address;
  /** data read over I2C bus */
  uint8_t m_data;
  /** error byte */
  int16_t m_error;
};

#endif // i2cNavigationKeyPad_H


