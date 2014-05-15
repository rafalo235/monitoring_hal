package com.hall.monitor.protocol;

public class SSensorData
{
  private long         idData;     // !< id pomiaru
  private char         idSensor;   // !< id czujnika
  private long         timeStamp;  // !< czas pomiaru
  private ESensorState sensorState; // !< stan czujnika
  private EDangerLevel dataState;  // !< stopien niebezpieczenstwa
  private SData        data;
  
  public SSensorData(long idData, char idSensor, long timeStamp,
      ESensorState sensorState, EDangerLevel dataState, SData data)
  {
    super();
    this.idData = idData;
    this.idSensor = idSensor;
    this.timeStamp = timeStamp;
    this.sensorState = sensorState;
    this.dataState = dataState;
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
  
  public EDangerLevel getDataState() {
    return dataState;
  }
  
  public SData getData() {
    return data;
  }
  
}
