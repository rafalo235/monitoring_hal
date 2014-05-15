package com.hall.monitor.protocol;

public class SProtocol
{
  private char         version;
  private long         size;
  private int          idConcentrator;
  private int          crc;
  private long         idPackage;
  private EMessageType type;
  private UMessage     message;
  
  public SProtocol(char version, long size, int idConcentrator, int crc,
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
    return size;
  }
  
  public int getIdConcentrator() {
    return idConcentrator;
  }
  
  public int getCrc() {
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
