package com.hall.monitor.protocol;
/**
 * Typ wartosci
 * @author Marcin Serwach
 *
 */
public enum EValueType
{
  INT_8(0, 8), UINT_8(1, 8), INT_16(2, 16), UINT_16(3, 16), INT_32(4, 32), UINT_32(
      5, 32), INT_64(6, 64), UINT_64(7, 64), FLOAT_32(8, 32), DOUBLE_64(9, 64), VOID(
      10, 8);
  /** wartosc w C++ - potrzebne do konwersji */
  private byte cppValue;
  /** rozmiar */
  private int sizeType;
  
  /**
   * Konstrutkor
   * @param cppValue wartosc w C++ 
   * @param sizeType rozmiar
   */
  private EValueType(int cppValue, int sizeType)
  {
    this.cppValue = (byte) cppValue;
    this.sizeType = sizeType;
  }
  
  /**
   * Zwraca byte C++
   * @return bajt C++
   */
  public byte getCppValue() {
    return cppValue;
  }
  
  /**
   * Zwraca dokladny rozmiar danej
   * @return rozmiar 
   */
  public int getTypeSizeBits() {
    return sizeType;
  }
  /**
   * Konwertuje wartosc z C++ na enum
   * @param cppValue wartosc z C++
   * @return typ wartosci
   */
  public static EValueType convert(int cppValue) {
    for (EValueType type : EValueType.values()) {
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
