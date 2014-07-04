package com.hall.monitor.protocol;

import java.util.ArrayList;

/**
 * Ustawienia konfiguracyjne
 * @author Marcin Serwach
 *
 */
public class CConfiguration
{
  // uint8_t configurationSize;
  /** rozmiar configurations */
  private char                           configurationSize;
  
  // std::vector<CConfigurationValue> configurations;
  /** wektor opcji konfiguracji */
  private ArrayList<CConfigurationValue> configurations;
  
  /** rozmiar naglowka */
  public static final int                HEADER_SIZE = EValueType.UINT_8.getTypeSizeBits() / 8;
  
  /**
   * Konstruktor
   * @param configurations wektor opcji konfiguracji
   */
  public CConfiguration(ArrayList<CConfigurationValue> configurations)
  {
    super();
    this.configurationSize = (char) configurations.size();
    this.configurations = configurations;
  }
  
  /**
   * Domyslny konstruktor
   */
  public CConfiguration()
  {
    super();

    this.configurations = new ArrayList<CConfigurationValue>();
    this.configurationSize = (char) configurations.size();
  }
  
  /**
   * Zwraca rozmiar configurations 
   * @return rozmiar configurations
   */
  public char getConfigurationSize() {
    return configurationSize;
  }
  
  /**
   * Zwraca wektor opcji konfiguracji
   * @return wektor opcji konfiguracji
   */
  public ArrayList<CConfigurationValue> getConfigurations() {
    return configurations;
  }
  
  /**
   * Zwraca rozmiar
   * @return rozmiar
   */
  public int getSize() {
    int size = 0;
    for (CConfigurationValue conf : configurations) {
      size += conf.getSize();
    }
    return HEADER_SIZE + size;
  }
}
