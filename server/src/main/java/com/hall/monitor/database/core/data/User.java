package com.hall.monitor.database.core.data;

import java.util.Set;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.OneToMany;
import javax.persistence.Table;

/**
 * Klasa wykorzystywana przy transformacji ORM z tabeli Users.
 * Klasa reprezentuje jednego uzytkownika.
 * @author Marcin Serwach
 *
 */
@Entity
@Table(name = "Users")
public class User
{
  /** id */
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idUser")
  private int                   idUser;
  
  /** imie uzytkownika */
  @Column(nullable = false, name = "firstName")
  private String                firstName;
  
  /** nazwisko uzytkownika */
  @Column(nullable = false, name = "lastName")
  private String                lastName;
  
  /** login */
  @Column(nullable = false, name = "login")
  private String                login;
  
  /** haslo */
  @Column(nullable = false, name = "password")
  private String                password;
  
  /** zbior konfiguracji, ktora uzytkownik zmienil */
  @OneToMany(mappedBy = "user")
  private Set<ConcentratorConf> configures;
  
  /**
   * Domyslny konstruktor
   */
  public User()
  {
  }
  
  /**
   * Konstruktor
   * @param firstName imie
   * @param lastName nazwisko
   * @param login login
   * @param password haslo
   */
  public User(String firstName, String lastName, String login, String password)
  {
    super();
    this.firstName = firstName;
    this.lastName = lastName;
    this.login = login;
    this.password = password;
  }
  
  /** 
   * Zmienia id
   * @param idUser id
   */
  public void setIdUser(int idUser) {
    this.idUser = idUser;
  }

  /**
   * Zwraca haslo
   * @return haslo
   */
  public String getPassword() {
    return password;
  }
  
  /**
   * Zmienia haslo
   * @param password haslo
   */
  public void setPassword(String password) {
    this.password = password;
  }
  
  /**
   * Zwraca imie
   * @return imie
   */
  public String getFirstName() {
    return firstName;
  }
  
  /**
   * Zmienia imie
   * @param firstName imie
   */
  public void setFirstName(String firstName) {
    this.firstName = firstName;
  }
  
  /**
   * Zwraca nazwisko
   * @return nazwisko
   */
  public String getLastName() {
    return lastName;
  }
  
  /**
   * Zmienia nazwisko
   * @param lastName nazwisko
   */
  public void setLastName(String lastName) {
    this.lastName = lastName;
  }
  
  /**
   * Zwraca login
   * @return login 
   */
  public String getLogin() {
    return login;
  }
  
  /**
   * Zmienia login
   * @param login login
   */
  public void setLogin(String login) {
    this.login = login;
  }
  
  /**
   * Zwraca id
   * @return id
   */
  public int getIdUser() {
    return idUser;
  }
  
}
