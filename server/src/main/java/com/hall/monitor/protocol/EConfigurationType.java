package com.hall.monitor.protocol;

/**
 * Typ opcji konfiguracji 
 * @author Marcin Serwach
 *
 */
public enum EConfigurationType
{
  SENDING_FREQUENCY(0), 
  SENSOR_TURN_ON(1), 
  CARD_SAVE(2), DANGER_LEVEL(3), ALARM_LEVEL(
      4), RESET(5), KEY_CHANGE(6), KEY_LIFETIME(7);
  
  /** wartosc w C++ - potrzebne do konwersji */
  private byte cppValue;
  
  /**
   * Konstrutkor
   * @param cppValue wartosc w C++ 
   */
  private EConfigurationType(int cppValue)
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
   * @return typ konfiguracji
   */
  public static EConfigurationType convert(int cppValue) {
    for (EConfigurationType type : EConfigurationType.values()) {
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