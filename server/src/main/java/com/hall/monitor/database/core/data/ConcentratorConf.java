package com.hall.monitor.database.core.data;

import java.util.Date;
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
 * Klasa wykorzystywana przy transformacji ORM z tabeli ConcentratorConfs.
 * Klasa reprezentuje jedna zmiane konfiguracji koncentratora.
 * @author Marcin Serwach
 *
 */
@Entity
@Table(name = "ConcentratorConfs")
public class ConcentratorConf
{
  /** id zmiany konfiguracji */
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idConcentratorConf")
  private int             idConcentratorConf;
  
  /** id paczki, której przesłana konfiguracja jest odpowiedzią */
  @Column(name = "idPackage")
  private long idPackage;
  
  /** koncentrator, którego konfiguracja jest zmieniana */
  @ManyToOne
  @JoinColumn(name = "idConcentrator")
  private Concentrator    concentrator;
  
  /** zbior zmian konfiguracji czujnikow/koncentratora */
  @OneToMany(mappedBy = "concentratorConf", fetch = FetchType.EAGER)
  private Set<SensorConf> sensorConf;
  
  /** Uzytkownik, ktory zmienil konfiguracje */
  @ManyToOne
  @JoinColumn(name = "idUser")
  private User            user;
  
  /** potwierdzenie zmiany: true - zmiana zostala dokonana; false zmiana czeka na przeslanie do koncentratora */
  @Column(name = "changed")
  private boolean         changed;
  
  /** data i czas ustalenie zmiany */
  @Column(name = "timeStamp")
  private Date            timeStamp;
  
  /** domyslny konstruktor */
  public ConcentratorConf()
  {
    
  }
  
  /**
   * Konstruktor
   * @param concentrator koncentratora, ktorego konfiguracja bedzie zmieniana
   * @param sensorConf zbior konfiguracji czujnikow/koncentratora
   * @param user uzytkownik, ktory ustalil konfiguracje
   * @param changed true jesli zostala konfiguracja ustawiona na koncentratorze; false jesli nie
   * @param timeStamp data i czas utworzenia konfiguracji
   */
  public ConcentratorConf(Concentrator concentrator,
      Set<SensorConf> sensorConf, User user, boolean changed, Date timeStamp)
  {
    super();
    this.concentrator = concentrator;
    this.sensorConf = sensorConf;
    this.user = user;
    this.changed = changed;
    this.timeStamp = timeStamp;
  }
  
  /**
   * Zmienia id konfiguracji
   * @param idConcentratorConf id konfiguracji
   */
  public void setIdConcentratorConf(int idConcentratorConf) {
    this.idConcentratorConf = idConcentratorConf;
  }

  /**
   * Zwraca koncentrator, ktorego konfiguracja dotyczy
   * @return koncentrator
   */
  public Concentrator getConcentrator() {
    return concentrator;
  }
  
  /**
   * Zmienia koncentrator, ktorego konfiguracja dotyczy
   * @param concentrator koncentrator
   */
  public void setConcentrator(Concentrator concentrator) {
    this.concentrator = concentrator;
  }
  
  /**
   * Zwraca id pakietu, ktorego konfiguracja jest odpowiedzia
   * @return id pakietu
   */
  public long getIdPackage() {
    return idPackage;
  }

  /**
   * Zmienai id pakietu, ktorego konfiguracja jest odpowiedzia
   * @param idPackage id pakietu
   */
  public void setIdPackage(long idPackage) {
    this.idPackage = idPackage;
  }

  /**
   * Zwraca zbior konfiguracji koncentrator/czujnikow
   * @return zbior konfiguracji
   */
  public Set<SensorConf> getSensorConf() {
    return sensorConf;
  }
  
  /**
   * Zmienia zbior konfiguracji koncentratora/czujnikow
   * @param sensorConf zbior konfiguracji koncentratora/czujnikow
   */
  public void setSensorConf(Set<SensorConf> sensorConf) {
    this.sensorConf = sensorConf;
  }
  
  /**
   * Zwraca uzytkownika, ktory ustalil konfiguracje
   * @return uzytkownik
   */
  public User getUser() {
    return user;
  }

  /**
   * Zmienai uzytkownika, ktory zmienil konfiguracje
   * @param user uzytkownik
   */
  public void setUser(User user) {
    this.user = user;
  }
  
  /**
   * Zwraca informacje czy zmiana zostala wprowadzona
   * @return true jesli konfiguracja zostala zmieniona na koncentratorze; false w przeciwnym wypadku
   */
  public boolean isChanged() {
    return changed;
  }
  
  /**
   * Zmienia informacje o zmnianie konfiguracji na koncentratorze
   * @param changed true konfiguracja zostala zmiona na koncentratorze; false nie
   */
  public void setChanged(boolean changed) {
    this.changed = changed;
  }
  
  /**
   * Zwraca date i czas ustalenie zmiany konfiguracji
   * @return data 
   */
  public Date getTimeStamp() {
    return timeStamp;
  }
  
  /**
   * Zmienia date i czas wprowadzenia zmiany konfiguracji
   * @param timeStamp data i czas wprowadzenia zmiany
   */
  public void setTimeStamp(Date timeStamp) {
    this.timeStamp = timeStamp;
  }
  
  /**
   * Zwraca id konfiguracji
   * @return id 
   */
  public int getIdConcentratorConf() {
    return idConcentratorConf;
  }
  
}
