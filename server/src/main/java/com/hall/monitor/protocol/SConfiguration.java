package com.hall.monitor.protocol;

import java.util.ArrayList;

public class SConfiguration
{
  private ArrayList<SConfigurationValue> configurations;
  
  public SConfiguration(ArrayList<SConfigurationValue> configurations)
  {
    super();

    this.configurations = configurations;
  }
  
  public char getConfigurationSize() {
    return (char) configurations.size();
  }
  
  public ArrayList<SConfigurationValue> getConfigurations() {
    return configurations;
  }
  
}
