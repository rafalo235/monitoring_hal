package com.hall.monitor.protocol;

public enum EDangerLevel
{
  NONE(0), DANGER(1), ALARM(2);
  private int cppValue;
  
  private EDangerLevel(int cppValue)
  {
    this.cppValue = cppValue;
  }
  
  public int getCppValue() {
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
}
