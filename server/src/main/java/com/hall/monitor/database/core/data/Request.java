package com.hall.monitor.database.core.data;

import java.util.Date;
import java.util.Set;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.Table;

/**
 * Klasa wykorzystywana przy transformacji ORM z tabeli Requests.
 * Klasa reprezentuje jedno zadanie przeslania konfiguracji.
 * @author Marcin Serwach
 *
 */
@Entity
@Table(name = "Requests")
public class Request
{
  /** id */
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idRequest")
  private int              idRequest;
  
  /** id paczki */
  @Column(name = "idPackage")
  private long idPackage;
  
  /** data i czas otrzymania */
  @Column(name = "receiveTime")
  private Date receiveTime;
  
  /** koncentrator, z ktorego pochodzi zadanie */
  @ManyToOne
  @JoinColumn(name = "idConcentrator")
  private Concentrator     concentrator;
  
  /** zbior konfiguracji */
  @OneToMany(mappedBy = "request", fetch = FetchType.EAGER)
  private Set<RequestConf> requestConfs;
  
  /** domyslny konstrutkor */
  public Request()
  {
  }
  
  /**
   * Konstruktor
   * @param idPackage id paczki 
   * @param receiveTime data i czas otrzymania
   * @param concentrator koncentrator, ktory wyslal zadanie
   * @param requestConfs zbior zadan
   */
  public Request(long idPackage, Date receiveTime, Concentrator concentrator,
      Set<RequestConf> requestConfs)
  {
    super();
    this.idPackage = idPackage;
    this.receiveTime = receiveTime;
    this.concentrator = concentrator;
    this.requestConfs = requestConfs;
  }

  /**
   * id paczki 
   * @return id paczki
   */
  public long getIdPackage() {
    return idPackage;
  }

  /**
   * Data i czas otrzymania paczki
   * @return data i czas
   */
  public Date getReceiveTime() {
    return receiveTime;
  }

  /**
   * Zmienia id 
   * @param idRequest id
   */
  public void setIdRequest(int idRequest) {
    this.idRequest = idRequest;
  }

  /**
   * zwraca id
   * @return id
   */
  public int getIdRequest() {
    return idRequest;
  }
  
  /**
   * Zwraca koncentrator, ktory wyslal zadanie
   * @return koncentrator
   */
  public Concentrator getConcentrator() {
    return concentrator;
  }
  
  /**
   * Zwraca zbior zadan
   * @return zbior zadan
   */
  public Set<RequestConf> getRequestConfs() {
    return requestConfs;
  }

  /**
   * zmienia id paczki
   * @param idPackage id paczki
   */
  public void setIdPackage(long idPackage) {
    this.idPackage = idPackage;
  }

  /**
   * Zmienia czas otrzymania zadanie
   * @param receiveTime czas otrzymania zadania
   */
  public void setReceiveTime(Date receiveTime) {
    this.receiveTime = receiveTime;
  }

  /**
   * Zmienia koncentrator, ktory wyslal zadanie
   * @param concentrator koncentrator
   */
  public void setConcentrator(Concentrator concentrator) {
    this.concentrator = concentrator;
  }

  /**
   * Zmienia zbior zadan
   * @param requestConfs zbior zadan
   */
  public void setRequestConfs(Set<RequestConf> requestConfs) {
    this.requestConfs = requestConfs;
  }
  
}
