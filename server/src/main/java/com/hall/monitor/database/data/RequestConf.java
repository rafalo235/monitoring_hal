package com.hall.monitor.database.data;

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

import org.hibernate.annotations.ForeignKey;

import com.hall.monitor.protocol.EConfigurationType;

@Entity
@Table(name = "RequestConfs")
public class RequestConf
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column
  private int        idRequestConf;
  
  @ManyToOne
  @JoinColumn(name = "idRequest")
  @ForeignKey(name = "FK_request1")
  private Request    request;
  
  @Column(nullable = false)
  @Enumerated(EnumType.STRING)
  private EConfigurationType configType;
  
  public RequestConf()
  {
  }
  
  public int getIdRequestConf() {
    return idRequestConf;
  }
  
  public Request getRequest() {
    return request;
  }
  
  public void setRequest(Request request) {
    this.request = request;
  }
  
  public EConfigurationType getConfigType() {
    return configType;
  }
  
  public void setConfigType(EConfigurationType configType) {
    this.configType = configType;
  }
  
}
