package com.hall.monitor.database.data;

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

@Entity
@Table(name = "Requests")
public class Request
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column
  private int             idRequest;
  
  @ManyToOne
  @JoinColumn(name = "idConcentrator")
  @ForeignKey(name="FK_concentrator2")
  private Concentrator    concentrator;
  
  @OneToMany
  private Set<RequestConf> requestConfs;
  
  public Request(){}

  public Request(Concentrator concentrator, Set<RequestConf> requestConfs)
  {
    super();
    this.concentrator = concentrator;
    this.requestConfs = requestConfs;
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
  
  
  
}
