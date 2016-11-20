#include <Arduino.h>
#include <Wire.h>
#include "i2cKeyPad.h"

i2cKeyPad::setup()
{
  Wire.begin();
  // init all ports for input
  write8(0xFF);
  
}


uint8_t i2cKeyPad::read8()
{
  Wire.beginTransmission(m_address);
  Wire.requestFrom(m_address, 1);
  m_data = Wire.receive();
  m_error = Wire.endTransmission();
  return _data;
}


void i2cKeyPad::write8(uint8_t value)
{
  Wire.beginTransmission(m_address);
  m_data = value;
  Wire.write(_data);
  m_error = Wire.endTransmission();
}





