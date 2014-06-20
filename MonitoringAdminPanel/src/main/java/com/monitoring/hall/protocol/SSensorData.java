package com.monitoring.hall.protocol;

public class SSensorData
{
  // uint32_t idData;
  private long            idData;                             // !< id pomiaru
                                                               
  // uint8_t idSensor;
  private char            idSensor;                           // !< id czujnika
                                                               
  // uint64_t timeStamp;
  private long            timeStamp;                          // !< czas
                                                               // pomiaru
                                                               
  // ESensorState sensorState;
  private ESensorState    sensorState;                        // !< stan
                                                               // czujnika
                                                               
  // EDangerLevel dangerLevel;
  private EDangerLevel    dangerLevel;                        // !< stopien
                                                               // niebezpieczenstwa
                                                               
  // SData data;
  private SData           data;
  
  public static final int HEADER_SIZE = (EValueType.UINT_32.getTypeSizeBits() + EValueType.UINT_8.getTypeSizeBits() + EValueType.UINT_64.getTypeSizeBits()
                                          + ESensorState.getSizeBits() + EDangerLevel
                                          .getSizeBits()) / 8;
  
  public SSensorData(long idData, char idSensor, long timeStamp,
      ESensorState sensorState, EDangerLevel dangerLevel, SData data)
  {
    super();
    this.idData = idData;
    this.idSensor = idSensor;
    this.timeStamp = timeStamp;
    this.sensorState = sensorState;
    this.dangerLevel = dangerLevel;
    this.data = data;
  }
  
  public long getIdData() {
    return idData;
  }
  
  public char getIdSensor() {
    return idSensor;
  }
  
  public long getTimeStamp() {
    return timeStamp;
  }
  
  public ESensorState getSensorState() {
    return sensorState;
  }
  
  public EDangerLevel getDangerLevel() {
    return dangerLevel;
  }
  
  public SData getData() {
    return data;
  }
  
  public int getSize() {
    return HEADER_SIZE + data.getSize();
  }
}
