package com.hall.monitor.database.data;

import java.util.Set;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.OneToMany;
import javax.persistence.Table;

@Entity
@Table(name = "Users")
public class User
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idUser")
  private int    idUser;
  
  @Column(nullable = false, name = "firstName")
  private String firstName;
  
  @Column(nullable = false, name = "lastName")
  private String lastName;
  
  @Column(nullable = false, name = "login")
  private String login;
  
  @OneToMany(mappedBy = "user")
  private Set<ConcentratorConf> configures;
  
  public User()
  {
  }
  
  public User(String firstName, String lastName, String login)
  {
    super();
    this.firstName = firstName;
    this.lastName = lastName;
    this.login = login;
  }
  
  public String getFirstName() {
    return firstName;
  }
  
  public void setFirstName(String firstName) {
    this.firstName = firstName;
  }
  
  public String getLastName() {
    return lastName;
  }
  
  public void setLastName(String lastName) {
    this.lastName = lastName;
  }
  
  public String getLogin() {
    return login;
  }
  
  public void setLogin(String login) {
    this.login = login;
  }
  
  public int getIdUser() {
    return idUser;
  }
  
}
