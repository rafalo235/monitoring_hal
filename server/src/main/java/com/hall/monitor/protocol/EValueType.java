package com.hall.monitor.protocol;

public enum EValueType
{
  INT_8(0), UINT_8(1), INT_16(2), UINT_16(3), INT_32(4), UINT_32(5), INT_64(6), UINT_64(
      7), FLOAT_32(8), DOUBLE_64(9), VOID(10);
  
  private int cppValue;
  
  private EValueType(int cppValue)
  {
    this.cppValue = cppValue;
  }
  
  public int getCppValue() {
    return cppValue;
  }
  
  public static EValueType convert(int cppValue) {
    for (EValueType type : EValueType.values()) {
      if (type.cppValue == cppValue) {
        return type;
      }
    }
    return null;
  }
}
