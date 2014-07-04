package com.hall.monitor.protocol;

import java.util.ArrayList;

/**
 * Odpowiedz serwera po otrzymaniu danych z czujnikow
 * @author Marcin Serwach
 *
 */
public class CServerResponse implements IMessage
{
  // EReceiveStatus status;
  /** Status otrzymanego pakietu z danymi z czujnikow */
  private EReceiveStatus  status;
  
  // uint32_t idRequestPackage;
  /** ID pakietu, ktory wyslal dane z czujnikow */
  private long            idRequestPackage;
  
  // SConfiguration configuration;
  /** Zadanie zmiany konfiguracji koncentratora */
  private CConfiguration  configuration;
  
  /** rozmiar naglowka */
  public static final int HEADER_SIZE = (EReceiveStatus.getSizeBits() + EValueType.UINT_32.getTypeSizeBits()) / 8;
  
  /**
   * Konstruktor
   * @param status Status otrzymanego pakietu z danymi z czujnikow
   * @param idRequestPackage ID pakietu, ktory wyslal dane z czujnikow
   * @param configuration Zadanie zmiany konfiguracji koncentratora
   */
  public CServerResponse(EReceiveStatus status, long idRequestPackage,
      CConfiguration configuration)
  {
    super();
    this.status = status;
    this.idRequestPackage = idRequestPackage;
    this.configuration = configuration;
  }
  
  /**
   * Konstruktor (sam tworzy pusta konfiguracje)
   * @param status Status otrzymanego pakietu z danymi z czujnikow
   * @param idRequestPackage  ID pakietu, ktory wyslal dane z czujnikow
   */
  public CServerResponse(EReceiveStatus status, long idRequestPackage)
  {
    super();
    this.status = status;
    this.idRequestPackage = idRequestPackage;
    ArrayList<CConfigurationValue> configurations = new ArrayList<CConfigurationValue>();
    this.configuration = new CConfiguration(configurations);
  }
  
  /**
   * Zwraca status otrzymanego pakietu z danymi z czujnikow
   * @return Status otrzymanego pakietu z danymi z czujnikow
   */
  public EReceiveStatus getStatus() {
    return status;
  }
  
  /**
   * Zwraca ID pakietu, ktory wyslal dane z czujnikow
   * @return ID pakietu, ktory wyslal dane z czujnikow
   */
  public long getIdRequestPackage() {
    return idRequestPackage;
  }
  
  /**
   * Zwraca zadanie zmiany konfiguracji koncentratora
   * @return Zadanie zmiany konfiguracji koncentratora
   */
  public CConfiguration getConfiguration() {
    return configuration;
  }
  
  @Override
  public int getSize() {
    
    return HEADER_SIZE + configuration.getSize();
  }
  
}
