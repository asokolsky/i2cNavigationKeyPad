#include <Arduino.h>
#include <Wire.h>
#include "i2cNavigationKeyPad.h"

void i2cNavigationKeyPad::setup()
{
  Wire.begin();
  // init all ports for input
  write8(0xFF);
  
}


uint8_t i2cNavigationKeyPad::read8()
{
  Wire.beginTransmission(m_address);
  Wire.requestFrom(m_address, 1);
  m_data = Wire.receive();
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


bool i2cNavigationKeyPad::readAndDispatch()
{
  uint8_t data = read8();
  
}


