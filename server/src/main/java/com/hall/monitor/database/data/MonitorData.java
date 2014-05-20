package com.hall.monitor.database.data;

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

import org.hibernate.annotations.ForeignKey;
import org.hibernate.annotations.OrderBy;

@Entity
@Table(name = "MonitorDatas")
public class MonitorData
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column
  private int             idMonitorData;
  
  // uint32_t idPackage;
  @Column
  private long            idPackage;
  
  // uint64_t sendTime;
  @Column(nullable = false)
  private Date            sendTime;
  
  @Column(nullable = false)
  private Date            receiveTime;
  
  // uint8_t sensorsAmount;
  @Column
  private int             sensorsAmount;
  
  @OneToMany
  @OrderBy(clause = "timeStamp")
  private Set<SensorData> sensorDatas;
  
  @ManyToOne
  @JoinColumn(name = "idConcentrator")
  @ForeignKey(name = "FK_concentrator4")
  private Concentrator    concentrator;
  
  public MonitorData()
  {
  }
  
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
  
  public void setIdMonitorData(int idMonitorData) {
    this.idMonitorData = idMonitorData;
  }

  public long getIdPackage() {
    return idPackage;
  }
  
  public void setIdPackage(long idPackage) {
    this.idPackage = idPackage;
  }
  
  public Date getSendTime() {
    return sendTime;
  }
  
  public void setSendTime(Date sendTime) {
    this.sendTime = sendTime;
  }
  
  public Date getReceiveTime() {
    return receiveTime;
  }
  
  public void setReceiveTime(Date receiveTime) {
    this.receiveTime = receiveTime;
  }
  
  public int getSensorsAmount() {
    return sensorsAmount;
  }
  
  public void setSensorsAmount(int sensorsAmount) {
    this.sensorsAmount = sensorsAmount;
  }
  
  public Set<SensorData> getSensorDatas() {
    return sensorDatas;
  }
  
  public void setSensorDatas(Set<SensorData> sensorDatas) {
    this.sensorDatas = sensorDatas;
  }
  
  public Concentrator getConcentrator() {
    return concentrator;
  }
  
  public void setConcentrator(Concentrator concentrator) {
    this.concentrator = concentrator;
  }
  
  public int getIdMonitorData() {
    return idMonitorData;
  }
  
}
