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
@Table(name = "Sensors")
public class Sensor
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column
  private int             idSensor;
  
  @ManyToOne
  @JoinColumn(name = "idConcentrator")
  @ForeignKey(name = "FK_concentrator3")
  private Concentrator    concentrator;
  
  @Column(nullable = false)
  private int             idConcentratorSensor;
  
  @OneToMany
  @OrderBy(clause = "timeStamp")
  private Set<SensorData> sensorDatas;
  
  public Sensor()
  {
  }
  
  public Sensor(Concentrator concentrator, int idConcentratorSensor)
  {
    super();
    this.concentrator = concentrator;
    this.idConcentratorSensor = idConcentratorSensor;
  }
  
  public Concentrator getConcentrator() {
    return concentrator;
  }
  
  public void setConcentrator(Concentrator concentrator) {
    this.concentrator = concentrator;
  }
  
  public int getIdSensor() {
    return idSensor;
  }
  
  public int getIdConcentratorSensor() {
    return idConcentratorSensor;
  }
  
  public Set<SensorData> getSensorDatas() {
    return sensorDatas;
  }
  
}
