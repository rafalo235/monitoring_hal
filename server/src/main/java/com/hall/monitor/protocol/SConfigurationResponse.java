package com.hall.monitor.protocol;

public class SConfigurationResponse implements UMessage
{
  private EReceiveStatus status;
  private long           idRequestPackage;
  private SConfiguration currentConfiguration;
  
  public SConfigurationResponse(EReceiveStatus status, long idRequestPackage,
      SConfiguration currentConfiguration)
  {
    super();
    this.status = status;
    this.idRequestPackage = idRequestPackage;
    this.currentConfiguration = currentConfiguration;
  }
  
  public EReceiveStatus getStatus() {
    return status;
  }
  
  public long getIdRequestPackage() {
    return idRequestPackage;
  }
  
  public SConfiguration getCurrentConfiguration() {
    return currentConfiguration;
  }
  
}
