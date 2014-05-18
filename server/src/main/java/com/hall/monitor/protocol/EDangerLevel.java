package com.hall.monitor.protocol;

public enum EDangerLevel
{
  NONE(0), DANGER(1), ALARM(2);
  private byte cppValue;
  
  private EDangerLevel(int cppValue)
  {
    this.cppValue = (byte) cppValue;
  }
  
  public byte getCppValue() {
    return cppValue;
  }
  
  public static EDangerLevel convert(int cppValue) {
    for (EDangerLevel type : EDangerLevel.values()) {
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
