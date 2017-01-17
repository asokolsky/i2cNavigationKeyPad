#ifndef i2cNavigationKeyPad_H
#define i2cNavigationKeyPad_H
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


/**
 * 
Possible addresses (from PCF8574 datasheet)

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

Yet this is what i2c scanner reports: (do I have a fake PCF8574?)

A2 A1 A0  I2C Addr
         Read Write
 0  0  0 0x21 0x20
 0  0  1 0x23 0x22
 0  1  0 0x45 0x44
 0  1  1 0x47 0x46
 1  0  0 0x49 0x48
 1  0  1 0x4B 0x4A
 1  1  0 0x4D 0x4C
 1  1  1 0x4F 0x4E
 
 
 */

/**
 *  Keypas keys - scan codes
 */
 
const uint8_t VK_NONE  = 0;
/** joystick buttons */
const uint8_t VK_UP    = 0x01;
const uint8_t VK_DOWN  = 0x02;
const uint8_t VK_LEFT  = 0x04;
const uint8_t VK_RIGHT = 0x08;
const uint8_t VK_SEL   = 0x10;
/** Custom context specific soft buttons */
const uint8_t VK_SOFTA = 0x20;
const uint8_t VK_SOFTB = 0x40;


class i2cNavigationKeyPad
{
public:
  i2cNavigationKeyPad(uint8_t addr) : m_address(addr)
  {
    
  }

  void setup();

  bool getAndDispatchKey(unsigned long ulNow);


protected:
  /** get readable names of the keyes pressed */
  const char *getKeyNames(uint8_t vks);

  /** call backs.  derive a class and overwrite those */
  virtual bool onUserInActivity(unsigned long ulNow);
  virtual bool onKeyAutoRepeat(uint8_t vks);
  virtual bool onKeyDown(uint8_t vks);
  virtual bool onLongKeyDown(uint8_t vks);
  virtual bool onKeyUp(uint8_t vks);

private:  
  /** delay in ms to autorepeat */
  const int iAutoRepeatDelay = 200;
  /** delay in ms before the long key is fired */
  const int iLongKeyDelay = 2000;
  /** inactivity timeout in milliseconds */
  const unsigned long ulInactivityDelay = 10000;

  /** when to fire long key */
  unsigned long m_ulToFireLongKey = 0;
  /** when to fire key auto repeat */
  unsigned long m_ulToFireAutoRepeat = 0;
  /** when inactivity timeout will happen */
  unsigned long m_ulToFireInactivity = ulInactivityDelay;

  uint8_t m_bOldKeys = VK_NONE;

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

  /** Delay inactivity notification */
  void onUserActivity(unsigned long ulNow) 
  {
    m_ulToFireInactivity = ulNow + ulInactivityDelay;
  }
  
};

#endif // i2cNavigationKeyPad_H

