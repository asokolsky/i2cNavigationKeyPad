#define NODEBUG 1

#include <Arduino.h>
#include <Wire.h>
#include "i2cNavigationKeyPad.h"
#include "Trace.h"

void i2cNavigationKeyPad::setup()
{
  DEBUG_PRINTLN("i2cNavigationKeyPad::setup()");
  Wire.begin();
  // init all ports for input
  //write8(0xFF);
  
}


uint8_t i2cNavigationKeyPad::read8()
{
  Wire.beginTransmission(m_address);
  Wire.requestFrom(m_address, (uint8_t)1);
  m_data = Wire.read();
  m_error = Wire.endTransmission();
  return m_data;
}


void i2cNavigationKeyPad::write8(uint8_t value)
{
  Wire.beginTransmission(m_address);
  m_data = value;
  Wire.write(m_data);
  m_error = Wire.endTransmission();
}

/*bool isKeyPressed(uint8_t vk, uint8_t data)
{
  //DEBUG_PRINT("isKeyPressed vk=0x"); DEBUG_PRINTHEX(vk); DEBUG_PRINT(" data=0x"); DEBUG_PRINTHEX(data); DEBUG_PRINT(" ~data=0x"); DEBUG_PRINTHEX(~data); // DEBUG_PRINT(" ~vk=0x"); DEBUG_PRINTHEX(~vk); 
  //DEBUG_PRINTLN("");
  return (vk & ~data) != 0;
}*/

const char *i2cNavigationKeyPad::getKeyNames(uint8_t vks)
{
  static char buf[40];
  buf[0] = '\0';
  if(vks & VK_UP)
    strcat(buf, "VK_UP ");
  if(vks & VK_DOWN)
    strcat(buf, "VK_DOWN ");
  if(vks & VK_LEFT)
    strcat(buf, "VK_LEFT ");
  if(vks & VK_RIGHT)
    strcat(buf, "VK_RIGHT ");
  if(vks & VK_SEL)
    strcat(buf, "VK_SEL ");
  if(vks & VK_SOFTA)
    strcat(buf, "VK_SOFTA ");
  if(vks & VK_SOFTB)
    strcat(buf, "VK_SOFTB ");
  return buf;
}



bool i2cNavigationKeyPad::getAndDispatchKey(unsigned long ulNow)
{
  //DEBUG_PRINTLN("i2cNavigationKeyPad::getAndDispatchKey()");
  read8();
  if(m_error != 0)
  {
    //DEBUG_PRINT("Read failed.  Error=0x"); DEBUG_PRINTHEX(m_error); DEBUG_PRINTLN("");
    return false;
  }
  /*
  if(m_data == 0xFF)
  {
    // no key pressed
    return false;
  }
  //DEBUG_PRINT("Read => 0x");DEBUG_PRINTHEX(m_data);DEBUG_PRINTLN("");
  DEBUG_PRINT("KeysPressed: ");
  if(isKeyPressed(VK_UP, m_data)) DEBUG_PRINT("VK_UP ");
  if(isKeyPressed(VK_DOWN, m_data)) DEBUG_PRINT("VK_DOWN ");
  if(isKeyPressed(VK_LEFT, m_data)) DEBUG_PRINT("VK_LEFT ");
  if(isKeyPressed(VK_RIGHT, m_data)) DEBUG_PRINT("VK_RIGHT ");
  if(isKeyPressed(VK_SEL, m_data)) DEBUG_PRINT("VK_SEL ");
  if(isKeyPressed(VK_SOFTA, m_data)) DEBUG_PRINT("VK_SOFTA ");
  if(isKeyPressed(VK_SOFTB, m_data)) DEBUG_PRINT("VK_SOFTB");
  DEBUG_PRINTLN("");
  */
  bool bRes = false;
  uint8_t vks = ~m_data;
  if(vks == m_bOldKeys) 
  {
    if(vks == VK_NONE)
    {
      if(ulNow < m_ulToFireInactivity)
        return false;
      bRes = onUserInActivity(ulNow);
      onUserActivity(ulNow);
      return bRes;
    }
    // fire auto repeat logic here
    if((m_ulToFireAutoRepeat == 0) || (ulNow < m_ulToFireAutoRepeat))
    {
      ;
    }
    else
    {
      m_ulToFireAutoRepeat = ulNow + iAutoRepeatDelay;
      //DEBUG_PRINT("onKeyAutoRepeat vks="); DEBUG_PRINT(getKeyName(vks)); DEBUG_PRINTLN("");
      bRes = onKeyAutoRepeat(vks);      
    }      
    // fire long key logic here
    if((m_ulToFireLongKey == 0) || (ulNow < m_ulToFireLongKey))
      return bRes;
    m_ulToFireLongKey = 0;
    //DEBUG_PRINT("onLongKeyDown vk="); DEBUG_PRINTLN(getKeyNames(vks));
    return onLongKeyDown(vks) || bRes;
  }
  // vks != m_OldKeys
  /*if(m_ulBounceSubsided == 0) {
    m_ulBounceSubsided = ulNow + s_iDebounceDelay;
    return false;
  }*/
  if(m_bOldKeys == VK_NONE) 
  {
    m_ulToFireLongKey = ulNow + iLongKeyDelay;
    m_ulToFireAutoRepeat = ulNow + iAutoRepeatDelay;
    DEBUG_PRINT("onKeyDown vk="); DEBUG_PRINTLN(getKeyNames(vks));
    bRes = onKeyDown(vks);
    onUserActivity(ulNow);
  }
  else if(vks != VK_NONE)
  {
    // ignore transients!
    //return false;
    
    // vks != m_bOldKeys
    // m_bOldKeys != VK_NONE
    // vks != VK_NONE
    uint8_t oldVKs = m_bOldKeys & ~vks;
    uint8_t newVKs = vks & ~m_bOldKeys;
    if(newVKs != 0)
    {
      m_ulToFireLongKey = ulNow + iLongKeyDelay;
      m_ulToFireAutoRepeat = ulNow + iAutoRepeatDelay;
      DEBUG_PRINT("onKeyDown newVKs="); DEBUG_PRINTLN(getKeyNames(newVKs));
      bRes = onKeyDown(vks);
      onUserActivity(ulNow);
    }
    else if(oldVKs != 0)
    {
      m_ulToFireAutoRepeat = m_ulToFireLongKey = 0;
      DEBUG_PRINT("onKeyUp oldVKs="); DEBUG_PRINTLN(getKeyNames(oldVKs));
      bRes = onKeyUp(oldVKs);
      onUserActivity(ulNow);
    }
  }
  else
  {
    m_ulToFireAutoRepeat = m_ulToFireLongKey = 0;
    DEBUG_PRINT("onKeyUp vk="); DEBUG_PRINT(getKeyNames(vks)); DEBUG_PRINT(" m_bOldKey="); DEBUG_PRINT(getKeyNames(m_bOldKeys)); DEBUG_PRINTLN("");
    bRes = onKeyUp(m_bOldKeys);
    onUserActivity(ulNow);
  }
  m_bOldKeys = vks;
  return bRes;
}

bool i2cNavigationKeyPad::onUserInActivity(unsigned long ulNow)
{
  //DEBUG_PRINT("i2cNavigationKeyPad::onUserInActivity ulNow="); DEBUG_PRINTDEC(ulNow); DEBUG_PRINTLN("");
  return false; 
}

bool i2cNavigationKeyPad::onKeyAutoRepeat(uint8_t vks)
{
  //DEBUG_PRINT("i2cNavigationKeyPad::onKeyAutoRepeat vks="); DEBUG_PRINTLN(getKeyNames(vks));
  return false; 
}

bool i2cNavigationKeyPad::onKeyDown(uint8_t vks)
{
  //DEBUG_PRINT("i2cNavigationKeyPad::onKeyDown vks="); DEBUG_PRINTLN(getKeyNames(vks));
  return false; 
}

bool i2cNavigationKeyPad::onLongKeyDown(uint8_t vks)
{
  //DEBUG_PRINT("i2cNavigationKeyPad::onLongKeyDown vks="); DEBUG_PRINTLN(getKeyNames(vks));
  return false; 
}

bool i2cNavigationKeyPad::onKeyUp(uint8_t vks)
{
  //DEBUG_PRINT("i2cNavigationKeyPad::onKeyUp vks="); DEBUG_PRINTLN(getKeyNames(vks));
  return false; 
}



