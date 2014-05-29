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
import com.hall.monitor.protocol.EValueType;

@Entity
@Table(name = "SensorConfs")
public class SensorConf
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idSensorConf")
  private int                idSensorConf;
  
  @ManyToOne
  @JoinColumn(name = "idSensor")
  private Sensor             sensor;
  
  @Column(nullable = false, name = "configType")
  @Enumerated(EnumType.STRING)
  private EConfigurationType configType;
  
  @Column(nullable = false, name = "type")
  @Enumerated(EnumType.STRING)
  private EValueType         type;
  
  @Column(nullable = false, name = "dataStr")
  private String             dataStr;
  
  @ManyToOne
  @JoinColumn(name = "idConcentratorConf")
  private ConcentratorConf             concentratorConf;
  
  public SensorConf()
  {
  }
  
  public SensorConf(Sensor sensor, EConfigurationType configType,
      EValueType type, String dataStr)
  {
    super();
    this.sensor = sensor;
    this.configType = configType;
    this.type = type;
    this.dataStr = dataStr;
  }
  
  public ConcentratorConf getConcentratorConf() {
    return concentratorConf;
  }

  public void setConcentratorConf(ConcentratorConf concentratorConf) {
    this.concentratorConf = concentratorConf;
  }

  public void setIdSensorConf(int idSensorConf) {
    this.idSensorConf = idSensorConf;
  }

  public void setSensor(Sensor sensor) {
    this.sensor = sensor;
  }

  public void setConfigType(EConfigurationType configType) {
    this.configType = configType;
  }

  public void setType(EValueType type) {
    this.type = type;
  }

  public void setDataStr(String dataStr) {
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
