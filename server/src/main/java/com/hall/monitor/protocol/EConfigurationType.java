package com.hall.monitor.protocol;

public enum EConfigurationType
{
  SENDING_FREQUENCY(0), SENSOR_TURN_ON(1), CARD_SAVE(2), DANGER_LEVEL(3), ALARM_LEVEL(4), RESET(5), KEY_CHANGE(6), KEY_LIFETIME(
      7);
  
  private int cppValue;
  
  private EConfigurationType(int cppValue)
  {
    this.cppValue = cppValue;
  }
  
  public int getCppValue() {
    return cppValue;
  }
  
  public static EConfigurationType convert(int cppValue) {
    for (EConfigurationType type : EConfigurationType.values()) {
      if (type.cppValue == cppValue) {
        return type;
      }
    }
    return null;
  }
}