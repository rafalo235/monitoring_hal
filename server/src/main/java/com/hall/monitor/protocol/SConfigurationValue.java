package com.hall.monitor.protocol;

public class SConfigurationValue
{
  // int8_t idSensor;
  private byte               idSensor;
  
  // EConfigurationType configurationType;
  private EConfigurationType configurationType;
  
  // SData data;
  private SData              data;
  
  public static final int    HEADER_SIZE = (EValueType.INT_8.getTypeSizeBits() + EConfigurationType
                                             .getSizeBits()) / 8;
  
  public SConfigurationValue(byte idSensor,
      EConfigurationType configurationType, SData data)
  {
    super();
    this.idSensor = idSensor;
    this.configurationType = configurationType;
    this.data = data;
  }
  
  public byte getIdSensor() {
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
