package com.hall.monitor.protocol;

import java.util.ArrayList;

/**
 * Prosba koncentratora o wyslanie konfiguracji
 * @author Marcin Serwach
 *
 */
public class CServerRequest implements IMessage
{
  // uint8_t requestsSize;
  /** rozmiar requests */
  private char                requestsSize;
  
  // std::vector<CRequest> requests;
  /** wektor zadan konfiguracji */
  private ArrayList<CRequest> requests;
  
  /** rozmiar naglowka */
  public static final int     HEADER_SIZE = (EValueType.UINT_8.getTypeSizeBits()) / 8;
  
  /**
   * Konstruktor
   * @param requests wektor zadan konfiguracji
   */
  public CServerRequest(ArrayList<CRequest> requests)
  {
    super();
    this.requests = requests;
    this.requestsSize = (char) requests.size();
  }
  
  /**
   * Zwraca rozmiar requests
   * @return rozmiar requests
   */
  public char getRequestsSize() {
    return requestsSize;
  }
  
  /**
   * Zwraca  wektor zadan konfiguracji
   * @return  wektor zadan konfiguracji
   */
  public ArrayList<CRequest> getRequests() {
    return requests;
  }
  
  @Override
  public int getSize() {
    int size = 0;
    for (CRequest req : requests) {
      size += req.getSize();
    }
    return HEADER_SIZE + size;
  }
  
}
