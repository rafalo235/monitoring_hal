package com.hall.monitor.protocol;
/**
 * Stan czujnika
 * @author Marcin Serwach
 *
 */
public enum ESensorState
{
  OK(0), TURN_OFF(1), BROKEN(2);
  
  /** wartosc w C++ - potrzebne do konwersji */
  private byte cppValue;
  
  /**
   * Konstrutkor
   * @param cppValue wartosc w C++ 
   */
  private ESensorState(int cppValue)
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
   * @return Stan czujnika
   */
  public static ESensorState convert(int cppValue) {
    for (ESensorState type : ESensorState.values()) {
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
