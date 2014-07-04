package com.hall.monitor.database.core.data;

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

/**
 * Klasa wykorzystywana przy transformacji ORM z tabeli Concentrators.
 * Klasa reprezentuje jeden koncentrator.
 * @author Marcin Serwach
 *
 */
@Entity
@Table(name = "Concentrators")
public class Concentrator
{
  /** id koncentratora */
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idConcentrator")
  private int         idConcentrator;
  
  /** hala, w ktorej znajduje sie koncentrator */
  @ManyToOne
  @JoinColumn(name = "idHall")
  private Hall        hall;
  
  /** zbior czujnikow podlaczonych do koncentratora */
  @OneToMany(mappedBy = "concentrator", fetch = FetchType.EAGER)
  private Set<Sensor> sensors;
  
  /** domyslny konstruktor */
  public Concentrator()
  {
  }
  
  /**
   * Konstruktor
   * @param hall hala, w ktorej znajduje sie koncentratora
   */
  public Concentrator(Hall hall)
  {
    super();
    this.hall = hall;
  }
  
  /**
   * Zmienia id koncentratora
   * @param idConcentrator nowe id
   */
  public void setIdConcentrator(int idConcentrator) {
    this.idConcentrator = idConcentrator;
  }
  
  /**
   * Zwraca hale, w ktorej znajduje sie koncentrator
   * @return hala, w ktorej znajduje sie kocnentrator
   */
  public Hall getHall() {
    return hall;
  }
  
  /**
   * Zmienia hale, w ktorej znajduje sie koncentrator
   * @param hall hala
   */
  public void setHall(Hall hall) {
    this.hall = hall;
  }
  
  /**
   * Zwraca zbior czujnikow podlaczonych do koncentratora
   * @return zbior czujnikow
   */
  public Set<Sensor> getSensors() {
    return sensors;
  }
  
  /**
   * Zmienia zbior czujnikow podlaczonych do koncentratora
   * @param sensors zbior czujnikow
   */
  public void setSensors(Set<Sensor> sensors) {
    this.sensors = sensors;
  }
  
  /**
   * Zwraca id koncentratora
   * @return id koncentratora
   */
  public int getIdConcentrator() {
    return idConcentrator;
  }
  
}
