
class i2cKeyPad
{

public:
  i2cKeyPad(int16_t addr = 0x38) : m_address(addr)
  {
    
  }

  void setup();

protected:
  uint8_t read8();
  void write8(uint8_t value); 

  int16_t getLastError() 
  {
    int16_t err = m_error;
    m_error = 0;
    return err;
  }

  int16_t m_address;
  uint8_t m_data;
  int16_t m_error;
};

