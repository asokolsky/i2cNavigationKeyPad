#include <Arduino.h>
#include "i2cNavigationKeyPad.h"

class MyNavKeyPad: public i2cNavigationKeyPad
{
public:  
  MyNavKeyPad() : i2cNavigationKeyPad(0x44)
  {
    
  }

  
};

MyNavKeyPad g_kp;

void setup()
{  
  g_kp.setup();
}

void loop()
{
  unsigned long ulNow = millis();
  g_kp.readAndDispatch();
 
}

