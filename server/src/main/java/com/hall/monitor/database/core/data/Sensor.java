package com.hall.monitor.database.core.data;

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
/**
 * Klasa wykorzystywana przy transformacji ORM z tabeli Sensors.
 * Klasa reprezentuje jeden czujnik oraz sam koncentrator. 
 * Obiekt Sensor reprezentuje koncentrator przy zmianie konfiguracji koncentratora, 
 * wtedy idConcentratorSensor przyjmuje wartosc 0xFF - jest to szczegolny przypadek. 
 * W przypadku tym obiekt Sensora i poleg jego concentrator odnosza sie do koncentratora. 
 * Ponadto wtedy sensorDatas jest pustym zbiorem.
 * @author Marcin Serwach
 *
 */
@Entity
@Table(name = "Sensors")
public class Sensor
{
  /** id czujnika w bazie */
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idSensor")
  private int             idSensor;
  
  /** koncentrator, do ktorego czujnik jest podlaczony */
  @ManyToOne
  @JoinColumn(name = "idConcentrator")
  private Concentrator    concentrator;
  
  /** id czujnika w koncentratorze (inne id niz idSensor) */
  @Column(nullable = false, name = "idConcentratorSensor")
  private int             idConcentratorSensor;
  
  /** zbior danych pomiarowych dla czujnika */
  @OneToMany(mappedBy = "sensor")
  private Set<SensorData> sensorDatas;
  
  /**
   * Domyslny konstruktor
   */
  public Sensor()
  {
  }
  
  /**
   * Konstruktor 
   * @param concentrator koncentrator, do ktorego czujnik jest podpiety
   * @param idConcentratorSensor id czujnika w koncentratorze
   */
  public Sensor(Concentrator concentrator, int idConcentratorSensor)
  {
    super();
    this.concentrator = concentrator;
    this.idConcentratorSensor = idConcentratorSensor;
  }
  
  /**
   * Zwraca koncentrator, do ktorego czujnik jest podpiety
   * @return koncentrator
   */
  public Concentrator getConcentrator() {
    return concentrator;
  }
  
  /**
   * Zmienia koncentrator
   * @param concentrator koncentrator
   */
  public void setConcentrator(Concentrator concentrator) {
    this.concentrator = concentrator;
  }
  
  /**
   * Zwraca id czujnika w bazie danych
   * @return id czujnika w bazie danych
   */
  public int getIdSensor() {
    return idSensor;
  }
  
  /**
   * Zwraca id czujnika w koncentratorze
   * @return id czujnika w koncentratorze
   */
  public int getIdConcentratorSensor() {
    return idConcentratorSensor;
  }
  
  /**
   * Zwraca zbior pomiarow z czujnika
   * @return zbior pomiarow
   */
  public Set<SensorData> getSensorDatas() {
    return sensorDatas;
  }
  
}
