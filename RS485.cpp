//
//    FILE: RS485.cpp
//  AUTHOR: Rob Tillaart
//    DATE: 30-okt-2017
// VERSION: 0.2.0
// PURPOSE: Arduino library for RS485 modules (MAX485)
//     URL: https://github.com/RobTillaart/RS485
//
// HISTORY:
// 0.1.x    2017-10-30  experimental versions.
// 0.2.0    2022-05-24  first published version


#include "RS485.h"


// - a stream - SWserial of HW serial (preferred)
// - a pin for TX- enable line
// - slave ID, or 0 as master (or don't care)
RS485::RS485(Stream * stream, uint8_t sendPin, uint8_t deviceID)
{
  _stream   = stream;
  _sendPin  = sendPin;
  _deviceID = deviceID;
  
  pinMode(_sendPin, OUTPUT);
  digitalWrite(_sendPin, LOW);   //  receiver mode
}


int RS485::available()
{
  return _stream->available();
}

int RS485::read()
{
  return _stream->read();
}

int RS485::peek()
{
  return _stream->peek();
}

void RS485::flush()
{
  _stream->flush();
}


void RS485::setMicrosPerByte(uint32_t baudRate)
{
  // count 11 bits / byte
  _microsPerByte = (11 * 1000000UL) / baudRate;
}



///////////////////////////////////////////////////////
//
//  EXPERIMENTAL - to be tested - use at own risk.
//
///////////////////////////////////////////////////////
//
// commando   value   meaning
//
//    SOH      0x01   start of header
//    STX      0x02   start of text
//    ETX      0x03   end of text
//
// optional
//    ACK      0x06   ACKnowledge
//    NAK      0x15   Not Acknowledge
//    CAN      0x18   CANcel
//
///////////////////////////////////////////////////////
//
// A message has the following layout
//
//     SOH          start of header
//     deviceID     to
//     deviceID     sender
//     length       length of message
//     STX          start of text
//     message      idem
//     CHECKSUM     idem, message only!
//     ETX          end of text
//


/*
void RS485::send(uint8_t deviceID, uint8_t msg[], uint8_t len)
{
  uint8_t CHKSUM = 0;
  digitalWrite(_enablePin, HIGH);
  _stream->write(SOH);
  _stream->write(deviceID);   // TO
  _stream->write(_deviceID);  // FROM
  _stream->write(len);        // LENGTH BODY
  _stream->write(STX);
  for (int i = 0; i < len; i++)
  {
    _stream->write(msg[i]);
    CHKSUM ^= msg[i];        // TODO good for now => CRC8 ?
    delayMicroseconds(_microsPerByte);
  }
  _stream->write(CHKSUM);
  delayMicroseconds(_microsPerByte);
  _stream->write(ETX);
  delayMicroseconds(_microsPerByte);
  digitalWrite(_enablePin, LOW);
}

//  returns true if complete message is captured.
//  multiple calls needed as it should be non-blocking.
bool RS485::receive(uint8_t &deviceID, uint8_t msg[], uint8_t &len)
{
  static uint8_t length = 0;
  static bool    forMe = false;
  uint8_t        CHKSUM = 0;

  if (_stream->available() == 0) return false;

  uint8_t v = _stream->read();

  if (v == SOH)
  {
    _bidx = 0;        // start new packet
    _buffer[_bidx++] = v;
    forMe = true;
    return false;
  }

  if (_bidx == 1)     // expect TO
  {
    forMe = (v == _deviceID);
    _buffer[_bidx++] = v;
    return false;
  }

  if (!forMe) return false;

  if (_bidx == 2)     // expect SENDER
  {
    _buffer[_bidx++] = v;
    return false;
  }

  if (_bidx == 3)     // expect LEN
  {
    length = v;
    _buffer[_bidx++] = v;
    return false;
  }

  if (_bidx == 4)     // expect STX
  {
    _buffer[_bidx++] = v;
    return false;
  }
  
  if (_bidx >= 48)    // prevent buffer overflow
  {
    len = 0;
    forMe = false;
    _bidx = 0;
    return false;
  }

  if (v == ETX)
  {
    for (int i = 0, j = 5; i < length; i++, j++)
    {
      msg[i] = _buffer[j];    // copy buffer
      CHKSUM ^= _buffer[j];
    }
    if (CHKSUM != _buffer[_bidx - 1])
    len = length;             // copy length
    deviceID = _buffer[2];    // copy sender
    forMe = false;
    _bidx = 0;
    return true;
  }
return false;
}
*/


//////////////////////////////////////////////////
//
//  PRIVATE
//
size_t RS485_HD::write(uint8_t c)
{
  digitalWrite(_sendPin, HIGH);   //  transmit mode
  size_t n = _stream->write(c);
  delayMicroseconds(_usPerByte);
  digitalWrite(_sendPin, LOW);    //  receiver mode
  return n;
}


//  TODO: fix blocking - yield()?
size_t RS485_HD::write(uint8_t * array, uint8_t length)
{
  digitalWrite(_sendPin, HIGH);   //  transmit mode
  size_t n = _stream->write(array, length);
  delayMicroseconds(_usPerByte);
  digitalWrite(_sendPin, LOW);    //  receiver mode
  return n;
}

// -- END OF FILE --

