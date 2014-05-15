package com.hall.monitor.protocol;

public enum EMessageType
{
  MONITOR_DATA(0), CONFIGURATION_RESPONSE(1), SERVER_REQUEST(2), SERVER_MONITOR_RESPONSE(
      3);
  private int cppValue;
  
  private EMessageType(int cppValue)
  {
    this.cppValue = cppValue;
  }
  
  public int getCppValue() {
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
}
