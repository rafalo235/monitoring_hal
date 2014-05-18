package com.hall.monitor.protocol;

public class SProtocol
{
  // uint8_t version;
  private char            version;
  
  // uint32_t size;
  private long            size;
  
  // uint16_t idConcentrator;
  private char             idConcentrator;
  
  // uint32_t idPackage;
  private long            idPackage;
  
  // EMessageType type;
  private EMessageType    type;
  
  // UMessage message;
  private UMessage        message;
  
  // uint16_t crc;
  private char             crc;
  
  public static final int HEADER_SIZE = (EValueType.UINT_8.getTypeSizeBits()
                                          + EValueType.UINT_32
                                              .getTypeSizeBits()
                                          + EValueType.UINT_16
                                              .getTypeSizeBits()
                                          + EValueType.UINT_32
                                              .getTypeSizeBits()
                                          + EMessageType.getSizeBits() + EValueType.UINT_16
                                          .getTypeSizeBits()) / 8;
  
  public SProtocol(char version, long size, char idConcentrator, char crc,
      long idPackage, EMessageType type, UMessage message)
  {
    super();
    this.version = version;
    this.size = size;
    this.idConcentrator = idConcentrator;
    this.crc = crc;
    this.idPackage = idPackage;
    this.type = type;
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
