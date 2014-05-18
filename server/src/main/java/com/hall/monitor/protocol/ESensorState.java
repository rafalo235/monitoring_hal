package com.hall.monitor.protocol;

public enum ESensorState
{
  OK(0), TURN_OFF(1), BROKEN(2);
  private byte cppValue;
  
  private ESensorState(int cppValue)
  {
    this.cppValue = (byte) cppValue;
  }
  
  public byte getCppValue() {
    return cppValue;
  }
  
  public static ESensorState convert(int cppValue) {
    for (ESensorState type : ESensorState.values()) {
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
