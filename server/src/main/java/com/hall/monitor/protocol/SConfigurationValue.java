package com.hall.monitor.protocol;

public class SConfigurationValue
{
  private byte               idSensor;
  private EConfigurationType configurationType;
  private SData              data;
  
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
  
}
