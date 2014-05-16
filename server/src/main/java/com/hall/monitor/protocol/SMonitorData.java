package com.hall.monitor.protocol;

import java.util.ArrayList;

public class SMonitorData implements UMessage
{
  private long                   sendTime;
  private char                  sensorsAmount;
  private ArrayList<SSensorData> sensorsData;
  
  public SMonitorData(long sendTime, char sensorsAmount, 
      ArrayList<SSensorData> sensorsData)
  {
    super();
    this.sendTime = sendTime;
    this.sensorsAmount = sensorsAmount;
    this.sensorsData = sensorsData;
  }
  
  public long getSendTime() {
    return sendTime;
  }
  
  public char getSensorsAmount() {
    return sensorsAmount;
  }
  
  public long getSensorsDataSize() {
    return sensorsData.size();
  }
  
  public ArrayList<SSensorData> getSensorsData() {
    return sensorsData;
  }
  
}
