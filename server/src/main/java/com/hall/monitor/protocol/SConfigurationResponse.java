package com.hall.monitor.protocol;

public class SConfigurationResponse implements UMessage
{
  // EReceiveStatus status;
  private EReceiveStatus  status;
  
  // uint32_t idRequestPackage;
  private long            idRequestPackage;
  
  // SConfiguration currentConfiguration;
  private SConfiguration  currentConfiguration;
  
  public static final int HEADER_SIZE = (EReceiveStatus.getSizeBits() + EValueType.UINT_32.getTypeSizeBits()) / 8;
  
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
  
  @Override
  public int getSize() {
    return HEADER_SIZE + currentConfiguration.getSize();
  }
  
}
