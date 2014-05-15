package com.hall.monitor.protocol;

public enum ESensorState
{
  OK(0), TURN_OFF(1), BROKEN(2);
  private int cppValue;
  
  private ESensorState(int cppValue)
  {
    this.cppValue = cppValue;
  }
  
  public int getCppValue() {
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
}
