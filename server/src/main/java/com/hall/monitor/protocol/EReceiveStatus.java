package com.hall.monitor.protocol;
/**
 * Status otrzymanej wiadomosci
 * @author Marcin Serwach
 *
 */
public enum EReceiveStatus
{
  OK(0), CRC_ERROR(1), BAD_STRUCTURE(2), OPERATION_FAILED(3);
  
  /** wartosc w C++ - potrzebne do konwersji */
  private byte cppValue;
  
  /**
   * Konstrutkor
   * @param cppValue wartosc w C++ 
   */
  private EReceiveStatus(int cppValue)
  {
    this.cppValue = (byte) cppValue;
  }
  
  /**
   * Zwraca byte C++
   * @return bajt C++
   */
  public byte getCppValue() {
    return cppValue;
  }
  
  /**
   * Konwertuje wartosc z C++ na enum
   * @param cppValue wartosc z C++
   * @return Status otrzymanej wiadomosci
   */
  public static EReceiveStatus convert(int cppValue) {
    for (EReceiveStatus type : EReceiveStatus.values()) {
      if (type.cppValue == cppValue) {
        return type;
      }
    }
    return null;
  }
  
  /**
   * Zwraca rozmiar
   * @return rozmiar
   */
  public static int getSizeBits() {
    return 8;
  }
}
