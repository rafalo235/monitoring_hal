package com.hall.monitor.protocol;

public class SServerResponse implements UMessage
{
  // EReceiveStatus status;
  private EReceiveStatus  status;
  
  // uint32_t idRequestPackage;
  private long            idRequestPackage;
  
  // SConfiguration configuration;
  private SConfiguration  configuration;
  
  public static final int HEADER_SIZE = (EReceiveStatus.getSizeBits() + EValueType.UINT_32.getTypeSizeBits()) / 8;
  
  public SServerResponse(EReceiveStatus status, long idRequestPackage,
      SConfiguration configuration)
  {
    super();
    this.status = status;
    this.idRequestPackage = idRequestPackage;
    this.configuration = configuration;
  }
  
  public EReceiveStatus getStatus() {
    return status;
  }
  
  public long getIdRequestPackage() {
    return idRequestPackage;
  }
  
  public SConfiguration getConfiguration() {
    return configuration;
  }
  
  @Override
  public int getSize() {
    
    return HEADER_SIZE + configuration.getSize();
  }
  
}
