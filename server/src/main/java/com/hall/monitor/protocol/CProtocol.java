package com.hall.monitor.protocol;

/**
 * Struktura protokolu.
 * @author Marcin Serwach
 *
 */
public class CProtocol
{
  public static final char VERSION     = (char) 1;
  
  public static final char ERROR_ID_CONCENTRATOR = (char) 0xFF;
  // uint8_t version;
  /** Wersja protkolu */
  private char             version;
  
  // uint32_t size;
  /** Rozmiar laczny danych przesylanych protokolem. */
  private long             size;
  
  // uint16_t idConcentrator;
  /** ID koncentratora */
  private char             idConcentrator;
  
  // uint32_t idPackage;
  /** ID danych pakietu danych. */
  private long             idPackage;
  
  // EMessageType type;
  /** Typ pakietu danych wysylanych protokolem */
  private EMessageType     type;
  
  // IMessage message;
  /** Dane wysylane protokolem.  */
  private IMessage         message;
  
  // uint16_t crc;
  /** Cykliczny kod nadmiarowy dla calej struktury */
  private char             crc;
  
  /** rozmiar naglowka */
  public static final int  HEADER_SIZE = (EValueType.UINT_8.getTypeSizeBits()
                                           + EValueType.UINT_32
                                               .getTypeSizeBits()
                                           + EValueType.UINT_16
                                               .getTypeSizeBits()
                                           + EValueType.UINT_32
                                               .getTypeSizeBits()
                                           + EMessageType.getSizeBits() + EValueType.UINT_16
                                           .getTypeSizeBits()) / 8;
  
  /**
   * Konstruktor
   * @param size Rozmiar laczny danych przesylanych protokolem.
   * @param idConcentrator ID koncentratora 
   * @param crc Cykliczny kod nadmiarowy dla calej struktury
   * @param idPackage ID danych pakietu danych.
   * @param type Typ pakietu danych wysylanych protokolem 
   * @param message  Dane wysylane protokolem. 
   */
  public CProtocol(long size, char idConcentrator, char crc, long idPackage,
      EMessageType     type, IMessage message)
  {
    this.version = VERSION;
    this.size = size;
    this.idConcentrator = idConcentrator;
    this.crc = crc;
    this.idPackage = idPackage;
    this.message = message;
    this.type = type;

  }
  
  /**
   * Konstruktor
   * @param size Rozmiar laczny danych przesylanych protokolem.
   * @param idConcentrator ID koncentratora 
   * @param crc Cykliczny kod nadmiarowy dla calej struktury
   * @param idPackage ID danych pakietu danych.
   * @param message  Dane wysylane protokolem. 
   */
  public CProtocol(long size, char idConcentrator, char crc, long idPackage,
      IMessage message)
  {
    super();
    this.version = VERSION;
    this.size = size;
    this.idConcentrator = idConcentrator;
    this.crc = crc;
    this.idPackage = idPackage;
    if (message instanceof CConfigurationResponse) {
      this.type = EMessageType.CONFIGURATION_RESPONSE;
    }
    else if (message instanceof CMonitorData) {
      this.type = EMessageType.MONITOR_DATA;
    }
    else if (message instanceof CServerResponse) {
      this.type = EMessageType.SERVER_MONITOR_RESPONSE;
    }
    else {
      this.type = EMessageType.SERVER_REQUEST;
    }
    this.message = message;
  }
  
  /**
   * Zwraca wersje protkolu
   * @return Wersja protkolu
   */
  public char getVersion() {
    return version;
  }
  
  /**
   * Zwraca rozmiar laczny danych przesylanych protokolem.
   * @return Rozmiar laczny danych przesylanych protokolem.
   */
  public long getSize() {
    if (size == 0) {
      size = HEADER_SIZE + message.getSize();
    }
    return size;
  }
  
  /**
   * Zwraca ID koncentratora 
   * @return ID koncentratora 
   */
  public char getIdConcentrator() {
    return idConcentrator;
  }
  
  /**
   * Zwraca cykliczny kod nadmiarowy dla calej struktury
   * @return Cykliczny kod nadmiarowy dla calej struktury
   */
  public char getCrc() {
    return crc;
  }
  
  /**
   * Zwraca ID danych pakietu danych.
   * @return ID danych pakietu danych.
   */
  public long getIdPackage() {
    return idPackage;
  }
  
  /**
   * Zwraca typ pakietu danych wysylanych protokolem 
   * @return Typ pakietu danych wysylanych protokolem 
   */
  public EMessageType getType() {
    return type;
  }
  
  /**
   * Zwraca dane wysylane protokolem.
   * @return Dane wysylane protokolem.
   */
  public IMessage getMessage() {
    return message;
  }
  
}
