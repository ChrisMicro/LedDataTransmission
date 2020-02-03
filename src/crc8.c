//from 
// 
// LEONARDOMILIANI.com
// Official site 
//
// http://www.leonardomiliani.com/2013/un-semplice-crc8-per-arduino/?lang=en
//
// modified by ChrisMicro Feb.2014
// type byte replaced by uint8_t
//
// CRC-8 - based on the CRC8 formulas by Dallas/Maxim
// code released under the therms of the GNU GPL 3.0 license
//
#include "crc8.h"

uint8_t crc8(const uint8_t *data, uint8_t len) 
{
  uint8_t tempI;
  uint8_t crc = 0x00;
  while (len--) {
    uint8_t extract = *data++;
    for (tempI = 8; tempI; tempI--) {
      uint8_t sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum) {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}
