package com.hall.monitor.database.core.data;

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

import com.hall.monitor.protocol.EDangerLevel;
import com.hall.monitor.protocol.ESensorState;
import com.hall.monitor.protocol.EValueType;

/**
 * Klasa wykorzystywana przy transformacji ORM z tabeli SensorDatas.
 * Klasa reprezentuje pojedynczy pomiar.
 * @author Marcin Serwach
 *
 */
@Entity
@Table(name = "SensorDatas")
public class SensorData
{
  /** id */
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idSensorData")
  private int          idSensorData;
  
  // uint32_t idData;
  /** id pomiaru */
  @Column(nullable = false, name = "idData")
  private long         idData;
  
  // uint8_t idSensor;
  /** czujnik, z ktorego pochodzi pomiar */
  @ManyToOne
  @JoinColumn(name = "idSensor")
  private Sensor       sensor;
  
  /** czas wykonania pomiaru */
  @Column(nullable = false, name = "timeStamp")
  private Date         timeStamp;
  
  /** typ danych pomiaru */
  @Column(nullable = false, name = "type")
  @Enumerated(EnumType.STRING)
  private EValueType   type;
  
  /** dana pomiaru w formie string - dane w tabeli sa trzymane w formie tekstu, 
   * co dalej niezaleznosc od typu danych otrzymanych z koncentratora. */
  @Column(nullable = false, name = "dataStr")
  private String       dataStr;
  
  /** otrzymany stan czujnika */
  @Column(name = "state")
  @Enumerated(EnumType.STRING)
  private ESensorState state;
  
  /** stopien niebezpieczenstwa */
  @Column(nullable = false, name = "dangerLevel")
  @Enumerated(EnumType.STRING)
  private EDangerLevel dangerLevel;
  
  /** paczka, w ktorej pomiary dotarly do serwera */
  @ManyToOne
  @JoinColumn(name = "idMonitorData")
  private MonitorData  monitorData;
  
  /**
   * domyslny konstruktor
   */
  public SensorData()
  {
  }
  
  /**
   * Konstruktor
   * @param idData id pomiaru
   * @param sensor czujnik, z ktrego pomiar pochodzi
   * @param timeStamp czas pomiaru
   * @param type typ danej z pomiaru
   * @param dataStr dana z pomiaru
   * @param state stan czujnika
   * @param dangerLevel poziom niebezpieczenstwa
   * @param monitorData paczka, w ktorej pomiar zostal przeslany
   */
  public SensorData(long idData, Sensor sensor, Date timeStamp,
      EValueType type, String dataStr, ESensorState state,
      EDangerLevel dangerLevel, MonitorData monitorData)
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
  
  /**
   * Zwraca id z bazy danych
   * @return id z bazy danych
   */
  public int getIdSensorData() {
    return idSensorData;
  }
  
  /**
   * zwraca id pomiaru
   * @return id pomiaru
   */
  public long getIdData() {
    return idData;
  }
  
  /**
   * Zwraca czujnik, z ktorego jest pomiar
   * @return czujnik
   */
  public Sensor getSensor() {
    return sensor;
  }
  
  /**
   * Zwraca czas pomiaru
   * @return czas pomiaru
   */
  public Date getTimeStamp() {
    return timeStamp;
  }
  
  /**
   * Zwraca typ danej pomiarowej
   * @return typ danej
   */
  public EValueType getType() {
    return type;
  }
  
  /**
   * Zwraca dana z pomiaru
   * @return data z pomiaru
   */
  public String getDataStr() {
    return dataStr;
  }
  
  /**
   * Zwraca stan czujnika
   * @return stan czujnika
   */
  public ESensorState getState() {
    return state;
  }
  
  /**
   * Zwraca poziom niebezpieczenstwa
   * @return poziom niebezpieczenstwa
   */
  public EDangerLevel getDangerLevel() {
    return dangerLevel;
  }
  
  /**
   * Zwraca serie pomiarowa, w ktorej byl przeslany pomiar
   * @return seria pomiarowa
   */
  public MonitorData getMonitorData() {
    return monitorData;
  }

  /**
   * Zmienia id z bazy danych
   * @param idSensorData id z bazy
   */
  public void setIdSensorData(int idSensorData) {
    this.idSensorData = idSensorData;
  }

  /**
   * Zmienia id pomiaru
   * @param idData id pomiaru
   */
  public void setIdData(long idData) {
    this.idData = idData;
  }

  /**
   * Zmienia czujnik, z ktrego pochodz pomiar
   * @param sensor czujnik
   */
  public void setSensor(Sensor sensor) {
    this.sensor = sensor;
  }

  /**
   * Zmienia czas pomiaru
   * @param timeStamp czas pomiaru
   */
  public void setTimeStamp(Date timeStamp) {
    this.timeStamp = timeStamp;
  }

  /**
   * Zmienia typ danej
   * @param type typ danej
   */
  public void setType(EValueType type) {
    this.type = type;
  }

  /**
   * Zmienia dana pomiarowa
   * @param dataStr dana pomiarowa
   */
  public void setDataStr(String dataStr) {
    this.dataStr = dataStr;
  }

  /**
   * Zmienia stan czujnika
   * @param state stan czujnia
   */
  public void setState(ESensorState state) {
    this.state = state;
  }

  /**
   * Zmienia poziom niebezpieczenstwa
   * @param dangerLevel poziom niebezpieczenstwa
   */
  public void setDangerLevel(EDangerLevel dangerLevel) {
    this.dangerLevel = dangerLevel;
  }

  /**
   * Zmienia paczke pomiarowa
   * @param monitorData paczka pomiarowa
   */
  public void setMonitorData(MonitorData monitorData) {
    this.monitorData = monitorData;
  }
  
  
}
