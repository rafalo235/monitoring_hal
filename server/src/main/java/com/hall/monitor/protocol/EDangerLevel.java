package com.hall.monitor.protocol;
/**
 * Poziom niebezpieczenstwa
 * @author Marcin Serwach
 *
 */
public enum EDangerLevel
{
  NONE(0), DANGER(1), ALARM(2);
  /** wartosc w C++ - potrzebne do konwersji */
  private byte cppValue;
  
  /**
   * Konstrutkor
   * @param cppValue wartosc w C++ 
   */
  private EDangerLevel(int cppValue)
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
   * @return Poziom niebezpieczenstwa
   */
  public static EDangerLevel convert(int cppValue) {
    for (EDangerLevel type : EDangerLevel.values()) {
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
