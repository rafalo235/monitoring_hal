package com.hall.monitor.protocol;

import java.util.ArrayList;

/**
 * Dane z czujnikow wysylane z koncentrator do serwera
 * @author Marcin Serwach
 *
 */
public class CMonitorData implements IMessage
{
  // uint64_t sendTime;
  /** czas pierwszej proby wyslania pakietu */
  private long                   sendTime;
  
  // uint8_t sensorsAmount;
  /** ilosc czujnikow */
  private char                   sensorsAmount;
  
  // uint32_t sensorsDataSize;
  /** rozmiar wektora z danymi czujnikow */
  private long                   sensorsDataSize;
  
  // std::vector<CSensorData> sensorsData;
  /** wektor z danymi z czujnikow */
  private ArrayList<CSensorData> sensorsData;
  
  /** rozmiar naglowka */
  public static final int        HEADER_SIZE = (EValueType.INT_64
                                                 .getTypeSizeBits()
                                                 + EValueType.UINT_8
                                                     .getTypeSizeBits() + EValueType.UINT_32
                                                 .getTypeSizeBits()) / 8;
  
  /**
   * Konstruktor 
   * @param sendTime czas pierwszej proby wyslania pakietu
   * @param sensorsAmount ilosc czujnikow
   * @param sensorsData wektor z danymi z czujnikow
   */
  public CMonitorData(long sendTime, char sensorsAmount,
      ArrayList<CSensorData> sensorsData)
  {
    super();
    this.sendTime = sendTime;
    this.sensorsAmount = sensorsAmount;
    this.sensorsData = sensorsData;
    this.sensorsDataSize = sensorsData.size();
  }
  
  /**
   * Zwraca czas pierwszej proby wyslania pakietu
   * @return czas pierwszej proby wyslania pakietu
   */
  public long getSendTime() {
    return sendTime;
  }
  
  /**
   * Zwraca ilosc czujnikow
   * @return ilosc czujnikow
   */
  public char getSensorsAmount() {
    return sensorsAmount;
  }
  
  /**
   * Zwraca rozmiar wektora z danymi czujnikow 
   * @return rozmiar wektora z danymi czujnikow
   */
  public long getSensorsDataSize() {
    return sensorsDataSize;
  }
  
  /**
   * Zwraca wektor z danymi z czujnikow
   * @return wektor z danymi z czujnikow
   */
  public ArrayList<CSensorData> getSensorsData() {
    return sensorsData;
  }
  
  /**
   * Zwraca rozmiar
   * @return rozmiar
   */
  @Override
  public int getSize() {
    int size = 0;
    for (CSensorData sensor : sensorsData) {
      size += sensor.getSize();
    }
    return HEADER_SIZE + size;
  }
  
}
