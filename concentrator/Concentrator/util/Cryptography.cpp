#include "Cryptography.h"
#include <memory>
#include <cstring>
#include "Memory.h"

#define RAFAL_ZOBACZ_BO_COS_NIE_DZIALA

namespace NUtil {

  uint16_t CCryptography::crc16(const char* data, int dataSize)
   {

     uint16_t crc = 0;
#ifdef RAFAL_ZOBACZ_BO_COS_NIE_DZIALA
     char divisor[3] = {static_cast<char>(0xFE),
                        static_cast<char>(0xEF),
                        static_cast<char>(0x80)}; //TODO

     std::shared_ptr<char> array = CMemory::getArray<char>(dataSize);
     char* copy = &*array;
     memcpy(copy, data, dataSize);

     xorBytes(copy, dataSize - 2, divisor);

     memcpy(&crc, copy, sizeof(uint16_t));
#endif
     return crc;
   }

   bool CCryptography::checkCrc16(const char *data, int dataSize) {

#ifdef RAFAL_ZOBACZ_BO_COS_NIE_DZIALA
     char divisor[3] = {static_cast<char>(0xFE),
                        static_cast<char>(0xEF),
                        static_cast<char>(0x80)}; //TODO
       std::shared_ptr<char> array = CMemory::getArray<char>(dataSize);
       char* copy = &*array;
       memcpy(copy, data, dataSize);

       xorBytes(copy, dataSize, divisor);

       for (int i = 0 ; i < dataSize ; i++) {
           if (copy[i] != 0) {
               return false;
           }
       }
#endif
       return true;
   }

   void CCryptography::xorBytes(char *data, int length, const char* divisor) {
       int pointer = length * 8;
       char tmp;

       do {
           for (int i = 0 ; i < 8 ; i++ ) {
               if (data[0] & 0x80) { // jeżeli najstarszy bit jest 1 oblicz XOR
                   data[0] ^= divisor[0];
                   data[1] ^= divisor[1];
                   tmp = data[2];
                   data[2] ^= divisor[2];
                   data[2] &= 0x80;
                   data[2] |= tmp & 0x7F;
               }
               shift(data, length); // przesuń bity danych o 1
               pointer--;
           }
       } while (pointer > 0);
   }

   void CCryptography::shift(char *data, int length) {
     data[0] = data[0] << 1;
     for (int i = 1 ; i < length ; i++ ) {
         if (data[i] & 0x80) {
             data[i - 1] |= 0x01;
         }
         data[i] = data[i] << 1;
     }
   }
}
