#include "Cryptography.h"
namespace NUtil {

  uint16_t CCryptography::crc16(const char* data, int dataSize)
  {
    uint16_t crc;
    int pointer = dataSize * 8;
    char divisor[3] = { 0xFE, 0xEF, 0x80 }; //TODO
    char *copy = (char*) calloc(dataSize, 1);
    char tmp;

    memcpy(copy, data, dataSize);

    do {
        for (int i = 0 ; i < 8 ; i++ ) {
            if (copy[0] & 0x80) { // jeżeli najstarszy bit jest 1 oblicz XOR
                copy[0] ^= divisor[0];
                copy[1] ^= divisor[1];
                tmp = copy[2];
                copy[2] ^= divisor[2];
                copy[2] &= 0x80;
                copy[2] |= tmp & 0x7F;
            }
            shift(copy, dataSize); // przesuń bity danych o 1
            pointer--;
        }
    } while (pointer > 0);

    memcpy(&crc, copy, sizeof(uint16_t));

    crc = htobe16(crc);

    free(copy);
    return crc;
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
