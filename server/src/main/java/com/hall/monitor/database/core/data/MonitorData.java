package com.hall.monitor.database.core.data;

import java.util.Date;
import java.util.Set;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.Table;

/**
 * Klasa wykorzystywana przy transformacji ORM z tabeli MonitorDatas.
 * Klasa reprezentuje jedna paczke przeslanych odczytow.
 * @author Marcin Serwach
 *
 */
@Entity
@Table(name = "MonitorDatas")
public class MonitorData
{
  /** id */
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idMonitorData")
  private int             idMonitorData;
  
  // uint32_t idPackage;
  /** id paczki */
  @Column(name = "idPackage")
  private long            idPackage;
  
  // uint64_t sendTime;
  /** data i czas wyslania z koncentrator */
  @Column(nullable = false, name = "sendTime")
  private Date            sendTime;
  
  /** data i czas otrzymania z koncentratora */
  @Column(nullable = false, name = "receiveTime")
  private Date            receiveTime;
  
  // uint8_t sensorsAmount;
  /** ilosc czujnikow */
  @Column(name = "sensorsAmount")
  private int             sensorsAmount;
  
  /** zbior danych pomiarowych z czujnikow */
  @OneToMany(mappedBy = "monitorData")
  private Set<SensorData> sensorDatas;
  
  /** koncentrator, ktorego dane obiekt zawiera */
  @ManyToOne
  @JoinColumn(name = "idConcentrator")
  private Concentrator    concentrator;
  
  /**
   * Domyslny konstruktor 
   */
  public MonitorData()
  {
  }
  
  /**
   * Konstruktor 
   * @param idPackage id paczki
   * @param sendTime data i czas wyslania z koncentratora
   * @param receiveTime data i czas otrzymania z koncentratora
   * @param sensorsAmount liczba czujnikow
   * @param concentrator koncentrator, z ktorego wyslano pomiary
   */
  public MonitorData(long idPackage, Date sendTime, Date receiveTime,
      int sensorsAmount, Concentrator concentrator)
  {
    super();
    this.idPackage = idPackage;
    this.sendTime = sendTime;
    this.receiveTime = receiveTime;
    this.sensorsAmount = sensorsAmount;
    this.concentrator = concentrator;
  }
  
  /**
   * Zmienia id 
   * @param idMonitorData id
   */
  public void setIdMonitorData(int idMonitorData) {
    this.idMonitorData = idMonitorData;
  }
  
  /**
   * Zwraca id paczki
   * @return id paczki
   */
  public long getIdPackage() {
    return idPackage;
  }
  
  /**
   * Zmienia id paczki 
   * @param idPackage id packzi
   */
  public void setIdPackage(long idPackage) {
    this.idPackage = idPackage;
  }
  
  /**
   * Zwraca date i czas wyslania
   * @return data i czas wyslania
   */
  public Date getSendTime() {
    return sendTime;
  }
  
  /**
   * Zmienia date i czas wyslania
   * @param sendTime data i czas wyslania
   */
  public void setSendTime(Date sendTime) {
    this.sendTime = sendTime;
  }
  
  /**
   * Zwraca date i czas odebrania
   * @return data i czas odebrania
   */
  public Date getReceiveTime() {
    return receiveTime;
  }
  
  /**
   * Zmienia date i czas odebrania
   * @param receiveTime data i czas odebrania
   */
  public void setReceiveTime(Date receiveTime) {
    this.receiveTime = receiveTime;
  }
  
  /**
   * Zwraca ilosc czujnikow
   * @return ilosc czujnikow
   */
  public int getSensorsAmount() {
    return sensorsAmount;
  }
  
  /**
   * Zmienia ilosc czujnikow
   * @param sensorsAmount ilosc czujnikow
   */
  public void setSensorsAmount(int sensorsAmount) {
    this.sensorsAmount = sensorsAmount;
  }
  
  /**
   * Zwraca zbior pomiarow
   * @return zbior pomiarow
   */
  public Set<SensorData> getSensorDatas() {
    return sensorDatas;
  }
  
  /**
   * Zmienia zbior pomiarow
   * @param sensorDatas zbior pomiarow
   */
  public void setSensorDatas(Set<SensorData> sensorDatas) {
    this.sensorDatas = sensorDatas;
  }
  
  /**
   * Zwraca koncentrator, z ktorego dane pochodza
   * @return koncentrator
   */
  public Concentrator getConcentrator() {
    return concentrator;
  }
  
  /**
   * Zmienia koncentrator, z ktorego pochodza dane
   * @param concentrator koncentrator
   */
  public void setConcentrator(Concentrator concentrator) {
    this.concentrator = concentrator;
  }
  
  /**
   * Zwraca id 
   * @return id
   */
  public int getIdMonitorData() {
    return idMonitorData;
  }
  
}
