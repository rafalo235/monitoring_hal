#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <cstdint>
#include <cstdlib>
#include <cstring>

//! \brief Namespace przydatnych klas.
namespace NUtil {

  //!
  //! \brief The CCryptography class klasa kryptografii ze statycznymi metodami
  class CCryptography
  {
    CCryptography() = delete;
  public:
    //!
    //! \brief crc16 wylicza crc16
    //! \param data dane
    //! \param dataSize rozmiar danych
    //! \return  CRC16
    static uint16_t crc16(const char* data, const int dataSize);

  private:
    static void shift(char* data, int length);

  };
}
#endif // CRYPTOGRAPHY_H
