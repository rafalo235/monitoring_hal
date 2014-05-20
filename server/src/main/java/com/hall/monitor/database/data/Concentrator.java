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
import org.hibernate.annotations.OrderBy;

@Entity
@Table(name = "Concentrators")
public class Concentrator
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column
  private int         idConcentrator;
  
  @ManyToOne
  @JoinColumn(name = "idHall")
  @ForeignKey(name = "FK_hall1")
  private Hall        hall;
  
  @OneToMany
  @OrderBy(clause = "idConcentatorSensor")
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
