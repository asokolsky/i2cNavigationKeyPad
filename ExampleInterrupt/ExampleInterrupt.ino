#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Trace.h"
#include "i2cNavigationKeyPad.h"

const byte MyNavKeyPadAddress = 0x27;

class MyNavKeyPad: public i2cNavigationKeyPad
{
public:  
  MyNavKeyPad() : i2cNavigationKeyPad(MyNavKeyPadAddress)
  {
    
  }
  bool onUserInActivity(unsigned long ulNow);
  bool onKeyAutoRepeat(uint8_t vks);
  bool onKeyDown(uint8_t vks);
  bool onLongKeyDown(uint8_t vks);
  bool onKeyUp(uint8_t vks);
};

bool MyNavKeyPad::onUserInActivity(unsigned long ulNow)
{
  DEBUG_PRINT("MyNavKeyPad::onUserInActivity ulNow="); DEBUG_PRINTDEC(ulNow); DEBUG_PRINTLN("");
  return false; 
}

bool MyNavKeyPad::onKeyAutoRepeat(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onKeyAutoRepeat vks="); DEBUG_PRINTLN(getKeyNames(vks));
  return false; 
}

bool MyNavKeyPad::onKeyDown(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onKeyDown vks="); DEBUG_PRINTLN(getKeyNames(vks));
  return false; 
}

bool MyNavKeyPad::onLongKeyDown(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onLongKeyDown vks="); DEBUG_PRINTLN(getKeyNames(vks));
  return false; 
}

bool MyNavKeyPad::onKeyUp(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onKeyUp vks="); DEBUG_PRINTLN(getKeyNames(vks));
  return false; 
}

MyNavKeyPad g_kp;
/** pin for keypad interrupts */
const uint8_t pinInterrupt = 2;
/** interrupt counter */
static volatile int g_iInterrupts = 0;

/** INT1 handler - pin 3 */
static void onInterrupt()
{
  g_iInterrupts++;
}


void setup()
{  
  Serial.begin(115200);
  
  delay(1000);   
  //while(!Serial)  ; // wait for serial port to connect. Needed for Leonardo only
  DEBUG_PRINTLN("i2cNavigationKeyPad (interrupt) test!");

  // read from the interrupt pin
  pinMode(pinInterrupt, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinInterrupt), onInterrupt, FALLING);
      
  g_kp.setup();
  
}

void loop()
{
  /** check keypad this often */
  const unsigned long ulKeypadCheckPeriod = 100;
  static unsigned long ulKeypadNextTimerCall = ulKeypadCheckPeriod;
  unsigned long ulNow = millis();
  if(g_iInterrupts > 0)
  {
    DEBUG_PRINT("g_iInterrupts="); DEBUG_PRINTDEC(g_iInterrupts); DEBUG_PRINTLN("");
    
    //cli();
    g_iInterrupts = 0;
    //sei();
    ulKeypadNextTimerCall = ulNow + ulKeypadCheckPeriod;
    // this will result in calls onKeyUp/onKeyDown but not much else
    g_kp.getAndDispatchKey(ulNow);
  }
  else if(ulNow > ulKeypadNextTimerCall)
  {
    ulKeypadNextTimerCall = ulNow + ulKeypadCheckPeriod;
    // this is necessary for calls onUserInActivity/onKeyAutoRepeat/onLongKeyDown
    g_kp.getAndDispatchKey(ulNow);
  }
  delay(1);
 
}

