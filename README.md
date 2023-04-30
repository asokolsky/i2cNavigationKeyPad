# I2C Navigation KeyPad Library

Simple [I2C](http://www.i2c-bus.org/i2c-primer/) keypad based on
[PCF8574](http://www.ti.com/product/PCF8574).
Has a central 5-way joystick and two (left and right) buttons.

![I2C Navigation KeyPad](doc/IMG_6622.jpg
"I2C Navigation KeyPad")

## Hardware

[Schematics and PCB](https://easyeda.com/asokolsky/I2C_Keypad-524d125bedb34150b7cb3deea4f5bff9)

### PinOut

| Pin | Name | Description |
| --- | -----| -----|
|1|VCC| Voltage supply.  Anywhere between 2.5V to 6V.  Connect to VCC of your controller.|
|2|SDA| Serial data line.|
|3|SCL| Serial clock line.|
|4|INT| Interrupt output, low level, generated when key is pressed.|
|5|GND| Ground|

Use pads SJ1, SJ2, SJ3 to set the peripheral I2C address
as described in [datasheet](http://www.ti.com/product/PCF8574/datasheet/detailed_description#SCPS0687144)


## Software Samples

[i2cNavigationKeyPad.ino](i2cNavigationKeyPad.ino)

### Polling

see [i2cNavigationKeyPad.ino](i2cNavigationKeyPad.ino)

### Interrupt Handling

n/a
