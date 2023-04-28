//    FILE: RS485_sniffer.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: simple sniffer prints data as hex dump
//     URL: https://github.com/RobTillaart/RS485

//  use a 2nd Serial port.
//  or only connect the RX of the MAX485.


#include "Arduino.h"
#include "RS485.h"


const uint8_t sendPin  = 4;    //  not used, listening only.
const uint8_t deviceID = 250;  //  not used, maybe in future.


//  use a 2nd Serial port.
//  or only connect the RX of the MAX485.
RS485 rs485(&Serial, sendPin, deviceID);


const uint8_t LED = 13;
uint8_t status = LOW;

uint32_t count = 0;


void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println(__FILE__);

  rs485.setMicrosPerByte(115200);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}


void loop()
{
  if (rs485.available() > 0)
  {
    //  toggle on receive
    digitalWrite(LED, !digitalRead(LED));

    if (count % 160 == 0)
    {
      Serial.println();
    }
    if (count % 16 == 0)
    {
      Serial.println();
      Serial.print(count);
      Serial.print('\t');
    }
    if (count % 4 == 0)
    {
      Serial.print(' ');
    }
    count++;

    int x = rs485.read();
    if (x < 0x10) Serial.print('0');
    Serial.print(x, HEX);
    Serial.print(" ");

    //  TODO PRINTABLE ASCII CHARS?
  }
}


//  -- END OF FILE --
