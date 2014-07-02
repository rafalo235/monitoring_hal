package com.hall.monitor.protocol;

public class SProtocol
{
  public static final char VERSION     = (char) 1;
  
  public static final char ERROR_ID_CONCENTRATOR = (char) 0xFF;
  // uint8_t version;
  private char             version;
  
  // uint32_t size;
  private long             size;
  
  // uint16_t idConcentrator;
  private char             idConcentrator;
  
  // uint32_t idPackage;
  private long             idPackage;
  
  // EMessageType type;
  private EMessageType     type;
  
  // UMessage message;
  private UMessage         message;
  
  // uint16_t crc;
  private char             crc;
  
  public static final int  HEADER_SIZE = (EValueType.UINT_8.getTypeSizeBits()
                                           + EValueType.UINT_32
                                               .getTypeSizeBits()
                                           + EValueType.UINT_16
                                               .getTypeSizeBits()
                                           + EValueType.UINT_32
                                               .getTypeSizeBits()
                                           + EMessageType.getSizeBits() + EValueType.UINT_16
                                           .getTypeSizeBits()) / 8;
  
  
  public SProtocol(long size, char idConcentrator, char crc, long idPackage,
      EMessageType     type, UMessage message)
  {
    this.version = VERSION;
    this.size = size;
    this.idConcentrator = idConcentrator;
    this.crc = crc;
    this.idPackage = idPackage;
    this.message = message;
    this.type = type;

  }
  public SProtocol(long size, char idConcentrator, char crc, long idPackage,
      UMessage message)
  {
    super();
    this.version = VERSION;
    this.size = size;
    this.idConcentrator = idConcentrator;
    this.crc = crc;
    this.idPackage = idPackage;
    if (message instanceof SConfigurationResponse) {
      this.type = EMessageType.CONFIGURATION_RESPONSE;
    }
    else if (message instanceof SMonitorData) {
      this.type = EMessageType.MONITOR_DATA;
    }
    else if (message instanceof SServerResponse) {
      this.type = EMessageType.SERVER_MONITOR_RESPONSE;
    }
    else {
      this.type = EMessageType.SERVER_REQUEST;
    }
    this.message = message;
  }
  
  public char getVersion() {
    return version;
  }
  
  public long getSize() {
    if (size == 0) {
      size = HEADER_SIZE + message.getSize();
    }
    return size;
  }
  
  public char getIdConcentrator() {
    return idConcentrator;
  }
  
  public char getCrc() {
    return crc;
  }
  
  public long getIdPackage() {
    return idPackage;
  }
  
  public EMessageType getType() {
    return type;
  }
  
  public UMessage getMessage() {
    return message;
  }
  
}
