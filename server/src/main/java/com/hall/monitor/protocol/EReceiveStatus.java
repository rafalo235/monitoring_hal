package com.hall.monitor.protocol;

public enum EReceiveStatus
{
  OK(0), CRC_ERROR(1), BAD_STRUCTURE(2), OPERATION_FAILED(3);
  private int cppValue;
  
  private EReceiveStatus(int cppValue)
  {
    this.cppValue = cppValue;
  }
  
  public int getCppValue() {
    return cppValue;
  }
  
  public static EReceiveStatus convert(int cppValue) {
    for (EReceiveStatus type : EReceiveStatus.values()) {
      if (type.cppValue == cppValue) {
        return type;
      }
    }
    return null;
  }
}
