package com.hall.monitor.protocol;

/**
 * Pojedyncze zadanie opcji konfigurowalnej
 * @author Marcin Serwach
 *
 */
public class CRequest
{
  public static final byte CONCENTRATOR_ID = (byte)0xFF;
  // uint8_t idSensor;
  /** id czujnika lub koncentratora */
  private char               idSensor;
  
  // EConfigurationType configurationType;
  /** opcja konfiguracji */
  private EConfigurationType configurationType;
  
  /** rozmiar naglowka */
  public static final int    HEADER_SIZE = (EValueType.INT_8.getTypeSizeBits() + EConfigurationType
                                             .getSizeBits()) / 8;
  
  /**
   * Konstruktor
   * @param idSensor id czujnika lub koncentratora 
   * @param configurationType opcja konfiguracji 
   */
  public CRequest(char idSensor, EConfigurationType configurationType)
  {
    super();
    this.idSensor = idSensor;
    this.configurationType = configurationType;
  }
  
  /**
   * Zwraca id czujnika lub koncentratora 
   * @return id czujnika lub koncentratora 
   */
  public char getIdSensor() {
    return idSensor;
  }
  
  /**
   * Zwraca opcje konfiguracji 
   * @return opcja konfiguracji 
   */
  public EConfigurationType getConfigurationType() {
    return configurationType;
  }
  
  public int getSize() {
    return HEADER_SIZE;
  }
}
