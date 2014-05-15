package com.hall.monitor.protocol;

public class SRequest
{
  private byte               idSensor;
  private EConfigurationType configurationType;
  
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
  
}
