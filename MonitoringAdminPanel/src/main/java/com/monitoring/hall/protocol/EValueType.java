package com.monitoring.hall.protocol;

public enum EValueType
{
  INT_8(0, 8), UINT_8(1, 8), INT_16(2, 16), UINT_16(3, 16), INT_32(4, 32), UINT_32(
      5, 32), INT_64(6, 64), UINT_64(7, 64), FLOAT_32(8, 32), DOUBLE_64(9, 64), VOID(
      10, 8);
  
  private byte cppValue;
  private int sizeType;
  
  private EValueType(int cppValue, int sizeType)
  {
    this.cppValue = (byte) cppValue;
    this.sizeType = sizeType;
  }
  
  public byte getCppValue() {
    return cppValue;
  }
  
  public int getTypeSizeBits() {
    return sizeType;
  }
  
  public static EValueType convert(int cppValue) {
    for (EValueType type : EValueType.values()) {
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
