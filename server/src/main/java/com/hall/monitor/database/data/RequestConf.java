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

import com.hall.monitor.protocol.EConfigurationType;

@Entity
@Table(name = "RequestsConfs")
public class RequestConf
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idRequestConf")
  private int                idRequestConf;
  
  
  @ManyToOne
  @JoinColumn(name = "idRequest")
  private Request            request;
 
  @Column(nullable = false, name = "configType")
  @Enumerated(EnumType.STRING)
  private EConfigurationType configType;
   
  public RequestConf()
  {
  }
  
  public RequestConf(Request request, EConfigurationType configType)
  {
    super();
    this.request = request;
    this.configType = configType;
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

  public void setIdRequestConf(int idRequestConf) {
    this.idRequestConf = idRequestConf;
  }

  
}
