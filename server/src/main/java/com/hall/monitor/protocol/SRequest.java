package com.hall.monitor.protocol;

public class SRequest
{
  // int8_t idSensor;
  private byte               idSensor;
  
  // EConfigurationType configurationType;
  private EConfigurationType configurationType;
  
  public static final int    HEADER_SIZE = (EValueType.INT_8.getTypeSizeBits() + EConfigurationType
                                             .getSizeBits()) / 8;
  
  public SRequest(byte idSensor, EConfigurationType configurationType)
  {
    super();
    this.idSensor = idSensor;
    this.configurationType = configurationType;
  }
  
  public byte getIdSensor() {
    return idSensor;
  }
  
  public EConfigurationType getConfigurationType() {
    return configurationType;
  }
  
  public int getSize() {
    return HEADER_SIZE;
  }
}
