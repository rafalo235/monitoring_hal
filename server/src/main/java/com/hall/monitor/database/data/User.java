package com.hall.monitor.database.data;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "Users")
public class User
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column
  private int    idUser;
  
  @Column(nullable = false)
  private String firstName;
  
  @Column(nullable = false)
  private String lastName;
  
  @Column(nullable = false)
  private String login;
  
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
