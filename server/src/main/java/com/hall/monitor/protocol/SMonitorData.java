package com.hall.monitor.protocol;

import java.util.ArrayList;

public class SMonitorData implements UMessage
{
  // uint64_t sendTime;
  private long                   sendTime;
  
  // uint8_t sensorsAmount;
  private char                   sensorsAmount;
  
  // uint32_t sensorsDataSize;
  private long                   sensorsDataSize;
  
  // SSensorData* sensorsData;
  private ArrayList<SSensorData> sensorsData;
  
  public static final int        HEADER_SIZE = (EValueType.INT_64
                                                 .getTypeSizeBits()
                                                 + EValueType.UINT_8
                                                     .getTypeSizeBits() + EValueType.UINT_32
                                                 .getTypeSizeBits()) / 8;
  
  public SMonitorData(long sendTime, char sensorsAmount,
      ArrayList<SSensorData> sensorsData)
  {
    super();
    this.sendTime = sendTime;
    this.sensorsAmount = sensorsAmount;
    this.sensorsData = sensorsData;
    this.sensorsDataSize = sensorsData.size();
  }
  
  public long getSendTime() {
    return sendTime;
  }
  
  public char getSensorsAmount() {
    return sensorsAmount;
  }
  
  public long getSensorsDataSize() {
    return sensorsDataSize;
  }
  
  public ArrayList<SSensorData> getSensorsData() {
    return sensorsData;
  }
  
  @Override
  public int getSize() {
    int size = 0;
    for (SSensorData sensor : sensorsData) {
      size += sensor.getSize();
    }
    return HEADER_SIZE + size;
  }
  
}
