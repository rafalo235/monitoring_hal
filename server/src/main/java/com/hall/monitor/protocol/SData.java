package com.hall.monitor.protocol;

public abstract class SData
{
  private EValueType type;
  private Object     value;
  
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
