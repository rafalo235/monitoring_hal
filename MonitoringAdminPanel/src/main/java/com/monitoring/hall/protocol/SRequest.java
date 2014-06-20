package com.monitoring.hall.protocol;

public class SRequest
{
  public static final byte CONCENTRATOR_ID = (byte)0xFF;
  // uint8_t idSensor;
  private char               idSensor;
  
  // EConfigurationType configurationType;
  private EConfigurationType configurationType;
  
  public static final int    HEADER_SIZE = (EValueType.INT_8.getTypeSizeBits() + EConfigurationType
                                             .getSizeBits()) / 8;
  
  public SRequest(char idSensor, EConfigurationType configurationType)
  {
    super();
    this.idSensor = idSensor;
    this.configurationType = configurationType;
  }
  
  public char getIdSensor() {
    return idSensor;
  }
  
  public EConfigurationType getConfigurationType() {
    return configurationType;
  }
  
  public int getSize() {
    return HEADER_SIZE;
  }
}
