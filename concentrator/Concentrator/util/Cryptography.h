#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <cstdint>

namespace NUtil {


  class CCryptography
  {
    CCryptography() = delete;
  public:
    static uint16_t crc16(const char* data, const int dataSize);

  };
}
#endif // CRYPTOGRAPHY_H
