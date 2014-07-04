package com.hall.monitor.protocol;

/**
 * Dane pojedynczego pomiaru.
 * 
 * @author Marcin Serwach
 * 
 */
public class CSensorData
{
  // uint32_t idData;
  /** id pomiaru */
  private long            idData;
  
  // uint8_t idSensor;
  /** id czujnika */
  private char            idSensor;
  
  // uint64_t timeStamp;
  /** czas pomiaru */
  private long            timeStamp;
  
  // ESensorState sensorState;
  /** stan czujnika */
  private ESensorState    sensorState;
  
  // EDangerLevel dangerLevel;
  /** stopien niebezpieczenstwa */
  private EDangerLevel    dangerLevel;
  
  // CData data;
  /** dana z pomiaru */
  private CData           data;
  
  /** rozmiar naglowka */
  public static final int HEADER_SIZE = (EValueType.UINT_32.getTypeSizeBits()
                                          + EValueType.UINT_8.getTypeSizeBits()
                                          + EValueType.UINT_64
                                              .getTypeSizeBits()
                                          + ESensorState.getSizeBits() + EDangerLevel
                                          .getSizeBits()) / 8;
  
  /**
   * Konstruktor
   * @param idData id pomairu
   * @param idSensor id czujnika
   * @param timeStamp czas pomiaru
   * @param sensorState stan czujnika
   * @param dangerLevel poziom niebezpieczenstwa
   * @param data dana pomiaru
   */
  public CSensorData(long idData, char idSensor, long timeStamp,
      ESensorState sensorState, EDangerLevel dangerLevel, CData data)
  {
    super();
    this.idData = idData;
    this.idSensor = idSensor;
    this.timeStamp = timeStamp;
    this.sensorState = sensorState;
    this.dangerLevel = dangerLevel;
    this.data = data;
  }
  
  /**
   * Zwraca id pomiaur
   * @return id pomiaru
   */
  public long getIdData() {
    return idData;
  }
  
  /**
   * Zwraca id czujnika
   * @return id czujnika
   */
  public char getIdSensor() {
    return idSensor;
  }
  
  /**
   * Zwraca czas pomiaru
   * @return czas pomiaru
   */
  public long getTimeStamp() {
    return timeStamp;
  }
  
  /**
   * Zwraca stan czujnika
   * @return stan czujnika
   */
  public ESensorState getSensorState() {
    return sensorState;
  }
  
  /**
   * Zwraca poziom niebezpieczenstwa
   * @return poziom niebezpieczenstwa
   */
  public EDangerLevel getDangerLevel() {
    return dangerLevel;
  }
  
  /**
   * Zwraca pomiar
   * @return pomiar
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
