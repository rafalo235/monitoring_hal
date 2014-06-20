package com.monitoring.hall.protocol;

public enum EMessageType
{
  MONITOR_DATA(0), CONFIGURATION_RESPONSE(1), SERVER_REQUEST(2), SERVER_MONITOR_RESPONSE(
      3);
  private byte cppValue;
  
  private EMessageType(int cppValue)
  {
    this.cppValue = (byte) cppValue;
  }
  
  public byte getCppValue() {
    return cppValue;
  }
  
  public static EMessageType convert(int cppValue) {
    for (EMessageType type : EMessageType.values()) {
      if (type.cppValue == cppValue) {
        return type;
      }
    }
    return null;
  }
  
  public static int getSizeBits() {
    return 8;
  }
}
