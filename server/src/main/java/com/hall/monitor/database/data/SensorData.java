package com.hall.monitor.database.data;

import java.util.Date;

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

import com.hall.monitor.protocol.EDangerLevel;
import com.hall.monitor.protocol.ESensorState;
import com.hall.monitor.protocol.EValueType;

@Entity
@Table(name = "SensorDatas")
public class SensorData
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column
  private int            idSensorData;
  
  // uint32_t idData;
  @Column(nullable=false)
  private long           idData;
  
  // uint8_t idSensor;
  @ManyToOne
  @JoinColumn(name = "idSensor")
  private Sensor         sensor;
  
  @Column(nullable=false)
  private Date           timeStamp;
  
  @Column(nullable=false)
  @Enumerated(EnumType.STRING)
  private EValueType      type;
  
  @Column(nullable=false)
  private String         dataStr;
  
  @Column
  @Enumerated(EnumType.STRING)
  private ESensorState    state;
  
  @Column(nullable=false)
  @Enumerated(EnumType.STRING)
  private EDangerLevel    dangerLevel;
  
  @ManyToOne
  @JoinColumn(name = "idMonitorData")
  @ForeignKey(name="FK_monitor1")
  private MonitorData monitorData;
  
  public SensorData()
  {
  }

  public SensorData(long idData, Sensor sensor, Date timeStamp, EValueType type,
      String dataStr, ESensorState state, EDangerLevel dangerLevel,
      MonitorData monitorData)
  {
    super();
    this.idData = idData;
    this.sensor = sensor;
    this.timeStamp = timeStamp;
    this.type = type;
    this.dataStr = dataStr;
    this.state = state;
    this.dangerLevel = dangerLevel;
    this.monitorData = monitorData;
  }

  public int getIdSensorData() {
    return idSensorData;
  }

  public long getIdData() {
    return idData;
  }

  public Sensor getSensor() {
    return sensor;
  }

  public Date getTimeStamp() {
    return timeStamp;
  }

  public EValueType getType() {
    return type;
  }

  public String getDataStr() {
    return dataStr;
  }

  public ESensorState getState() {
    return state;
  }

  public EDangerLevel getDangerLevel() {
    return dangerLevel;
  }

  public MonitorData getMonitorData() {
    return monitorData;
  }
 
  
  
}
