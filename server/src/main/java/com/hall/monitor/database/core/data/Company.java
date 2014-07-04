package com.hall.monitor.database.core.data;

import java.util.Set;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.OneToMany;
import javax.persistence.Table;

/**
 * Klasa wykorzystywana przy transformacji ORM z tabeli Companies.
 * Klasa reprezentuje jedna firme, do ktorej naleza hale.
 * @author Marcin Serwach
 *
 */
@Entity
@Table(name = "Companies")
public class Company
{
  /** id firmy */
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idCompany")
  private int       idCompany;
  
  /** nazwa firmy */
  @Column(nullable = false, name = "companyName")
  private String    companyName;
  
  /** adres firmy */
  @Column(nullable = false, name = "companyAddress")
  private String    companyAddress;
  
  /** Zbior hal, ktorych wlascicielem jest firma */
  @OneToMany(mappedBy = "company", fetch = FetchType.EAGER)
  private Set<Hall> halls;
  
  /** domyslny konstruktor */
  public Company()
  {
  }
  
  /**
   * Konstruktor
   * @param companyName nazwa firmy
   * @param companyAddress adres firmy
   */
  public Company(String companyName, String companyAddress)
  {
    super();
    this.companyName = companyName;
    this.companyAddress = companyAddress;
  }
  
  /**
   * Zwraca nazwa firmy
   * @return nazwa firmy
   */
  public String getCompanyName() {
    return companyName;
  }
  
  /**
   * Zmienia nazwe firmy
   * @param companyName nowa nazwa firmy
   */
  public void setCompanyName(String companyName) {
    this.companyName = companyName;
  }
  
  /**
   * Zmienia id firmy
   * @param idCompany nowe id firy
   */
  public void setIdCompany(int idCompany) {
    this.idCompany = idCompany;
  }
  
  /** 
   * Zwraca adres firmy
   * @return adres firmy
   */
  public String getCompanyAddress() {
    return companyAddress;
  }
  
  /**
   * Zmienia adres firmy
   * @param companyAddress nowy adres firmy
   */
  public void setCompanyAddress(String companyAddress) {
    this.companyAddress = companyAddress;
  }
  
  /**
   * Zwraca zbior hal danej firmy
   * @return zbior hal
   */
  public Set<Hall> getHalls() {
    return halls;
  }
  
  /**
   * Zmiania zbior hal firmy
   * @param halls nowy zbior hal
   */
  public void setHalls(Set<Hall> halls) {
    this.halls = halls;
  }
  
  /**
   * Zwraca id firmy
   * @return id firmy
   */
  public int getIdCompany() {
    return idCompany;
  }
  
}
