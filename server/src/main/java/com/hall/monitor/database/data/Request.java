package com.hall.monitor.database.data;

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

@Entity
@Table(name = "Requests")
public class Request
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idRequest")
  private int              idRequest;
  
  @Column(name = "idPackage")
  private long idPackage;
  
  @Column(name = "receiveTime")
  private Date receiveTime;
  
  @ManyToOne
  @JoinColumn(name = "idConcentrator")
  private Concentrator     concentrator;
  
  @OneToMany(mappedBy = "request", fetch = FetchType.EAGER)
  private Set<RequestConf> requestConfs;
  
  public Request()
  {
  }
  
  
  public Request(long idPackage, Date receiveTime, Concentrator concentrator,
      Set<RequestConf> requestConfs)
  {
    super();
    this.idPackage = idPackage;
    this.receiveTime = receiveTime;
    this.concentrator = concentrator;
    this.requestConfs = requestConfs;
  }


  public long getIdPackage() {
    return idPackage;
  }


  public Date getReceiveTime() {
    return receiveTime;
  }


  public void setIdRequest(int idRequest) {
    this.idRequest = idRequest;
  }


  public int getIdRequest() {
    return idRequest;
  }
  
  public Concentrator getConcentrator() {
    return concentrator;
  }
  
  public Set<RequestConf> getRequestConfs() {
    return requestConfs;
  }


  public void setIdPackage(long idPackage) {
    this.idPackage = idPackage;
  }


  public void setReceiveTime(Date receiveTime) {
    this.receiveTime = receiveTime;
  }


  public void setConcentrator(Concentrator concentrator) {
    this.concentrator = concentrator;
  }


  public void setRequestConfs(Set<RequestConf> requestConfs) {
    this.requestConfs = requestConfs;
  }
  
}
