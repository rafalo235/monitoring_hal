package com.hall.monitor.protocol;

/**
 * Potwierdzenie zmiany konfiguracji koncentratora do serwera 
 * @author Marcin Serwach
 *
 */
public class CConfigurationResponse implements IMessage
{
  // EReceiveStatus status;
  /** Status otrzymanego pakietu konfiguracyjnego lub status zmiany konfiguracji */
  private EReceiveStatus  status;
  
  // uint32_t idRequestPackage;
  /** ID pakietu, ktory zglosil zmiane konfiguracji  */
  private long            idRequestPackage;
  
  // SConfiguration currentConfiguration;
  /** Konfiguracja koncentratora */
  private CConfiguration  currentConfiguration;
  
  /** rozmiar naglowka */
  public static final int HEADER_SIZE = (EReceiveStatus.getSizeBits() + EValueType.UINT_32.getTypeSizeBits()) / 8;
  
  /**
   * Konstruktor
   * @param status status otrzymanego pakietu
   * @param idRequestPackage ID pakietu, ktory zglosil zmiane konfiguracji
   * @param currentConfiguration  Konfiguracja koncentratora
   */
  public CConfigurationResponse(EReceiveStatus status, long idRequestPackage,
      CConfiguration currentConfiguration)
  {
    super();
    this.status = status;
    this.idRequestPackage = idRequestPackage;
    this.currentConfiguration = currentConfiguration;
  }
  
  /**
   * Zwraca status otrzymanego pakietu
   * @return Status otrzymanego pakietu
   */
  public EReceiveStatus getStatus() {
    return status;
  }
  
  /**
   * Zwraca ID pakietu, ktory zglosil zmiane konfiguracji
   * @return ID pakietu, ktory zglosil zmiane konfiguracji
   */
  public long getIdRequestPackage() {
    return idRequestPackage;
  }
  
  /**
   * Zwraca konfiguracje koncentratora
   * @return  Konfiguracja koncentratora
   */
  public CConfiguration getCurrentConfiguration() {
    return currentConfiguration;
  }
  
  /**
   * Zwraca rozmiar
   * @return rozmiar
   */
  @Override
  public int getSize() {
    return HEADER_SIZE + currentConfiguration.getSize();
  }
  
}
