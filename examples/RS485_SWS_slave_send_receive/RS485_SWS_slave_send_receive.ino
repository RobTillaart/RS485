//    FILE: RS485_slave_send_receive.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo slave of send / receive
//     URL: https://github.com/RobTillaart/RS485

//  VERY EXPERIMENTAL

//  This is the code for a slave (to be used with master example)
//  Best is it to use on a MEGA or another board with multiple hardware Serials.
//  The master should be connected with two slaves
//  slave 2 is waiting for temperature requests.
//  slave 1 is waiting for humidity requests.
//
//  Note that Serial is used for debugging messages, so better use
//  a 2nd Serial for the RS485 bus. 
//  otherwise some binary messages will show up in the Serial monitor.


#include "Arduino.h"
#include "RS485.h"
#include "SoftwareSerial.h"

const uint8_t sendPin  = 4;
const uint8_t deviceID = 1;

SoftwareSerial SWS(6,7);  //  RX, TX


//  use Software Serial
RS485 rs485(&SWS, sendPin, deviceID);


//  for receiving (must be global)
uint8_t ID;
uint8_t buffer[32];
uint8_t len;


void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("RS485_LIB_VERSION: ");
  Serial.println(RS485_LIB_VERSION);

  SWS.begin(38400);
}


void loop()
{
  if (rs485.receive(ID, buffer, len))
  {
    buffer[len] = 0;
    Serial.print("RECV: ");
    Serial.println((char*) buffer);

    if (strcmp((char*)buffer, "Get Humidity") == 0)
    {
      int humidity = 50 + random(10);
      sprintf((char*)buffer, "HUM: %d", humidity);
      rs485.send(ID, buffer, strlen((char*)buffer));
      //  tidy up output
      Serial.println();
    }
    if (strcmp((char*)buffer, "Get Temperature") == 0)
    {
      int temperature = 15 + random(10);
      sprintf((char*)buffer, "TEM: %d", temperature);
      rs485.send(ID, buffer, strlen((char*)buffer));
      //  tidy up output
      Serial.println();
    }
  }
}


//  -- END OF FILE --
