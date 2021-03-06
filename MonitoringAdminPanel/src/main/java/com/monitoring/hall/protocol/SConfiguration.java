package com.monitoring.hall.protocol;

import java.util.ArrayList;

public class SConfiguration
{
  // uint8_t configurationSize;
  private char                           configurationSize;
  
  // SConfigurationValue* configurations;
  private ArrayList<SConfigurationValue> configurations;
  
  public static final int                HEADER_SIZE = EValueType.UINT_8.getTypeSizeBits() / 8;
  
  public SConfiguration(ArrayList<SConfigurationValue> configurations)
  {
    super();
    this.configurationSize = (char) configurations.size();
    this.configurations = configurations;
  }
  public SConfiguration()
  {
    super();

    this.configurations = new ArrayList<SConfigurationValue>();
    this.configurationSize = (char) configurations.size();
  }
  public char getConfigurationSize() {
    return configurationSize;
  }
  
  public ArrayList<SConfigurationValue> getConfigurations() {
    return configurations;
  }
  
  public int getSize() {
    int size = 0;
    for (SConfigurationValue conf : configurations) {
      size += conf.getSize();
    }
    return HEADER_SIZE + size;
  }
}
