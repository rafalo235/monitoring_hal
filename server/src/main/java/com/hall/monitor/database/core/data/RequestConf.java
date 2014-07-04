package com.hall.monitor.database.core.data;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;

import com.hall.monitor.protocol.EConfigurationType;
/**
 * Klasa wykorzystywana przy transformacji ORM z tabeli RequestsConfs.
 * Klasa reprezentuje zadanie pojedynczej opcji z konfiguracji - czesc zadania {@link Request}.
 * @author Marcin Serwach
 *
 */
@Entity
@Table(name = "RequestsConfs")
public class RequestConf
{
  /** id */
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idRequestConf")
  private int                idRequestConf;
  
  /** Zadanie, ktorego obiekt jest czescia */
  @ManyToOne
  @JoinColumn(name = "idRequest")
  private Request            request;
 
  /** typ konfiguracji */
  @Column(nullable = false, name = "configType")
  @Enumerated(EnumType.STRING)
  private EConfigurationType configType;
  
  /**
   * Konstruktor domyslny
   */
  public RequestConf()
  {
  }
  
  /**
   * Konstruktor
   * @param request zadanie
   * @param configType typ zadanen konfiguracji
   */
  public RequestConf(Request request, EConfigurationType configType)
  {
    super();
    this.request = request;
    this.configType = configType;
  }

  /**
   * zwraca id 
   * @return id
   */
  public int getIdRequestConf() {
    return idRequestConf;
  }

  /**
   * Zwraca zadanie, ktorego czescia jest obiekt
   * @return zadanie
   */
  public Request getRequest() {
    return request;
  }

  /**
   * Zmienia zadanie, ktorego czesci jest obiekt
   * @param request zadanie
   */
  public void setRequest(Request request) {
    this.request = request;
  }

  /**
   * Zwraca typ konfiguracji zadania
   * @return typ konfiguracji
   */
  public EConfigurationType getConfigType() {
    return configType;
  }
  
  /**
   * Zmienia typ konfiguracji
   * @param configType typ konfiguracji
   */
  public void setConfigType(EConfigurationType configType) {
    this.configType = configType;
  }

  /**
   * Zmienia id
   * @param idRequestConf id
   */
  public void setIdRequestConf(int idRequestConf) {
    this.idRequestConf = idRequestConf;
  }

  
}
