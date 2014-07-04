package com.hall.monitor.protocol;
/**
 * Typ wiadomosci
 * @author Marcin Serwach
 *
 */
public enum EMessageType
{
  MONITOR_DATA(0), CONFIGURATION_RESPONSE(1), SERVER_REQUEST(2), SERVER_MONITOR_RESPONSE(
      3);
  
  /** wartosc w C++ - potrzebne do konwersji */
  private byte cppValue;
  
  /**
   * Konstrutkor
   * @param cppValue wartosc w C++ 
   */
  private EMessageType(int cppValue)
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
   * @return typ wiadomosci
   */
  public static EMessageType convert(int cppValue) {
    for (EMessageType type : EMessageType.values()) {
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
