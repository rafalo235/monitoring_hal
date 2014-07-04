package com.hall.monitor.protocol;

/**
 * Pojedyncza wartosc konfigurowalna.
 * @author Marcin Serwach
 *
 */
public class CConfigurationValue
{
  // uint8_t idSensor;
  /** id czujnika lub koncentratora  */
  private char               idSensor;
  
  // EConfigurationType configurationType;
  /** opcja konfiguracji */
  private EConfigurationType configurationType;
  
  // SData data;
  /** wartosc dla danej opcji */
  private CData              data;
  
  /** rozmiar naglowka */
  public static final int    HEADER_SIZE = (EValueType.INT_8.getTypeSizeBits() + EConfigurationType
                                             .getSizeBits()) / 8;
  
  /**
   * Konstruktor
   * @param idSensor id czujnika lub koncentratora
   * @param configurationType opcja konfiguracji
   * @param data wartosc dla danej opcji
   */
  public CConfigurationValue(char idSensor,
      EConfigurationType configurationType, CData data)
  {
    super();
    this.idSensor = idSensor;
    this.configurationType = configurationType;
    this.data = data;
  }
  
  /**
   * Zwraca id czujnika lub koncentratora
   * @return id czujnika lub koncentratora
   */
  public char getIdSensor() {
    return idSensor;
  }
  
  /**
   * Zwraca opcje konfigurowalna
   * @return opcja konfigurowalna
   */
  public EConfigurationType getConfigurationType() {
    return configurationType;
  }
  
  /**
   * Zwraca wartosc dla danej opcji
   * @return wartosc dla danej opcji
   */
  public CData getData() {
    return data;
  }
  
  /**
   * Zwraca rozmiar
   * @return rozmiar
   */
  public int getSize() {
    return HEADER_SIZE + data.getSize();
  }
  
}
