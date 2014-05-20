package com.hall.monitor.database.data;

import java.util.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;

@Entity
@Table(name = "ReceivePackages")
public class ReceivePackage
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idPackage")
  private int          idPackage;
  
  @Column(name = "receiveDate")
  private Date         receiveDate;
  
  @ManyToOne
  @JoinColumn(name = "idConcentrator")
  private Concentrator concentrator;
  
  public ReceivePackage()
  {
  }
  
  public int getIdPackage() {
    return idPackage;
  }
  
  public void setIdPackage(int idPackage) {
    this.idPackage = idPackage;
  }
  
  public Date getReceiveDate() {
    return receiveDate;
  }
  
  public void setReceiveDate(Date receiveDate) {
    this.receiveDate = receiveDate;
  }
  
  public Concentrator getConcentrator() {
    return concentrator;
  }
  
  public void setConcentrator(Concentrator concentrator) {
    this.concentrator = concentrator;
  }
  
}
