package com.hall.monitor.protocol;

public class SConfigurationValue
{
  // uint8_t idSensor;
  private char               idSensor;
  
  // EConfigurationType configurationType;
  private EConfigurationType configurationType;
  
  // SData data;
  private SData              data;
  
  public static final int    HEADER_SIZE = (EValueType.INT_8.getTypeSizeBits() + EConfigurationType
                                             .getSizeBits()) / 8;
  
  public SConfigurationValue(char idSensor,
      EConfigurationType configurationType, SData data)
  {
    super();
    this.idSensor = idSensor;
    this.configurationType = configurationType;
    this.data = data;
  }
  
  public char getIdSensor() {
    return idSensor;
  }
  
  public EConfigurationType getConfigurationType() {
    return configurationType;
  }
  
  public SData getData() {
    return data;
  }
  
  public int getSize() {
    return HEADER_SIZE + data.getSize();
  }
  
}
