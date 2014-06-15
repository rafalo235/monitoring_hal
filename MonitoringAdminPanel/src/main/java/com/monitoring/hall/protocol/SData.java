package com.monitoring.hall.protocol;

public class SData
{
  public static final Short VOID_VALUE = 0xFF;
  public static final SData VOID       = new SData(EValueType.VOID, VOID_VALUE);
  
  // EValueType type;
  private EValueType        type;
  
  // UValue value;
  private Object            value;
  
  public SData(EValueType type, Object value)
  {
    super();
    this.type = type;
    this.value = value;
  }
  
  public EValueType getType() {
    return type;
  }
  
  public Object getValue() {
    return value;
  }
  
  public int getSize() {
    return (EValueType.getSizeBits() + type.getTypeSizeBits()) / 8;
  }
}
