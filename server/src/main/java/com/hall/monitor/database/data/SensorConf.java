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
import com.hall.monitor.protocol.EValueType;

@Entity
@Table(name = "SensorConfs")
public class SensorConf
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column
  private int        idSensorConf;
  
  @ManyToOne
  @JoinColumn(name = "idSensor")
  @ForeignKey(name = "FK_sensor1")
  private Sensor     sensor;
  
  @Column(nullable = false)
  @Enumerated(EnumType.STRING)
  private EConfigurationType configType;
  
  @Column(nullable = false)
  @Enumerated(EnumType.STRING)
  private EValueType  type;
  
  @Column(nullable = false)
  private String     dataStr;
  
  public SensorConf()
  {
  }
  
  public SensorConf(Sensor sensor, EConfigurationType configType, EValueType type,
      String dataStr)
  {
    super();
    this.sensor = sensor;
    this.configType = configType;
    this.type = type;
    this.dataStr = dataStr;
  }
  
  public int getIdSensorConf() {
    return idSensorConf;
  }
  
  public Sensor getSensor() {
    return sensor;
  }
  
  public EConfigurationType getConfigType() {
    return configType;
  }
  
  public EValueType getType() {
    return type;
  }
  
  public String getDataStr() {
    return dataStr;
  }
  
}
