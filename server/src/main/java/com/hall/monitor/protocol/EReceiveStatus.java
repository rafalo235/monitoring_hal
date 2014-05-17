package com.hall.monitor.protocol;

public enum EReceiveStatus
{
  OK(0), CRC_ERROR(1), BAD_STRUCTURE(2), OPERATION_FAILED(3);
  private byte cppValue;
  
  private EReceiveStatus(int cppValue)
  {
    this.cppValue = (byte) cppValue;
  }
  
  public byte getCppValue() {
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
  
  public static int getSizeBits() {
    return 8;
  }
}
