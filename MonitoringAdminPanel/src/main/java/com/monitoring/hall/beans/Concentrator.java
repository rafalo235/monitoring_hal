package com.monitoring.hall.beans;

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
@Table(name = "Concentrators")
public class Concentrator
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idConcentrator")
  private int         idConcentrator;
  
  @ManyToOne
  @JoinColumn(name = "idHall")
  private Hall        hall;
  
  @OneToMany(mappedBy = "concentrator", fetch = FetchType.EAGER)
  private Set<Sensor> sensors;
  
  public Concentrator()
  {
  }
  
  public Concentrator(Hall hall)
  {
    super();
    this.hall = hall;
  }
  
  public void setIdConcentrator(int idConcentrator) {
    this.idConcentrator = idConcentrator;
  }
  
  public Hall getHall() {
    return hall;
  }
  
  public void setHall(Hall hall) {
    this.hall = hall;
  }
  
  public Set<Sensor> getSensors() {
    return sensors;
  }
  
  public void setSensors(Set<Sensor> sensors) {
    this.sensors = sensors;
  }
  
  public int getIdConcentrator() {
    return idConcentrator;
  }
  
}
