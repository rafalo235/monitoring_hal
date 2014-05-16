package com.hall.monitor.protocol;

public class SData
{
  public static final Short VOID_VALUE = 0xFF;
  public static final SData VOID = new SData(EValueType.VOID, VOID_VALUE);
  
  private EValueType        type;
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
  
}
