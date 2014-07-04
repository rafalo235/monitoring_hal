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
 * Klasa wykorzystywana przy transformacji ORM z tabeli Halls.
 * Klasa reprezentuje jedna hale.
 * @author Marcin Serwach
 *
 */
@Entity
@Table(name = "Halls")
public class Hall
{
  /** id hali */
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idHall")
  private int               idHall;
  
  /** nazwa hali */
  @Column(nullable = false, name = "hallName")
  private String            hallName;
  
  /** adres hali */
  @Column(nullable = false, name = "address")
  private String            hallAddress;
  
  /** firma, do ktorej nalezy hala */
  @ManyToOne
  @JoinColumn(name = "idCompany")
  private Company           company;
  
  /** zbior koncentratorow w hali */
  @OneToMany(mappedBy = "hall", fetch = FetchType.EAGER)
  private Set<Concentrator> concentrators;
  
  /** domyslny konstrutkor */
  public Hall()
  {
  }
  
  /**
   * Konstruktor 
   * @param hallName nazwa hali 
   * @param hallAddress adres hali
   * @param company firma, do ktorej nalezy hala
   */
  public Hall(String hallName, String hallAddress, Company company)
  {
    super();
    this.hallName = hallName;
    this.hallAddress = hallAddress;
    this.company = company;
  }
  
  /**
   * Zmienai id hali
   * @param idHall id hali
   */
  public void setIdHall(int idHall) {
    this.idHall = idHall;
  }
  
  /**
   * Zwraca nazwe hali
   * @return nazwa hali
   */
  public String getHallName() {
    return hallName;
  }
  
  /**
   * Zmienia nazwe hali
   * @param hallName nazwa hali
   */
  public void setHallName(String hallName) {
    this.hallName = hallName;
  }
  
  /**
   * Zwraca adres hali
   * @return adres hali
   */
  public String getHallAddress() {
    return hallAddress;
  }
  
  /**
   * Zmienia adres hali
   * @param hallAddress adres hali
   */
  public void setHallAddress(String hallAddress) {
    this.hallAddress = hallAddress;
  }
  
  /**
   * Zwraca firme, do ktorej nalezy hala
   * @return firma
   */
  public Company getCompany() {
    return company;
  }
  
  /**
   * Zmienia firme, do ktorej nalezy hala
   * @param company firma
   */
  public void setCompany(Company company) {
    this.company = company;
  }
  
  /**
   * Zwraca zbior koncentratorow w hali
   * @return zbior koncentratorow
   */
  public Set<Concentrator> getConcentrators() {
    return concentrators;
  }
  
  /**
   * Zmienia zbior koncentratorow
   * @param concentrators zbior koncentratorow
   */
  public void setConcentrators(Set<Concentrator> concentrators) {
    this.concentrators = concentrators;
  }
  
  /**
   * Zwraca id hali
   * @return id hali
   */
  public int getIdHall() {
    return idHall;
  }
  
}
