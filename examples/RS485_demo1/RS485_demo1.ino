//
//    FILE: RS485_demo1.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo

//  connect 2 boards via RS485 and install this program on both.
//  adapt the ID's from the sender and recipient.


#include "RS485.h"


uint8_t m[20] = "hello";
uint8_t msg[20];
uint8_t len;
uint8_t ID;

RS485 RS485(&Serial, 4, 0);   // <<<<<<< 0 replace with local ID


void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  RS485.begin(115200);
  RS485.send(1, m, 5);   //  <<<  replace 1 with recipient ID
}


void loop()
{
  if (RS485.receive(ID, msg, len) == true)  //  recipient should get a toggling LED.
  {
    for (int i = 0; i < 100; i++)
    {
      digitalWrite(13, !digitalRead(13));
      delay(100);
    }
  }
  delay(1000);
}


//  -- END OF FILE --

