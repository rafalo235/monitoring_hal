package com.hall.monitor.database.data;

import java.util.Set;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.OneToMany;
import javax.persistence.OrderBy;
import javax.persistence.Table;

@Entity
@Table(name = "Companies")
public class Company
{
  @Id
  @GeneratedValue(strategy=GenerationType.AUTO)
  @Column
  private int idCompany;
  
  @Column(nullable=false)
  private String name;
  
  @Column(nullable=false)
  private String companyAddress;
  
  @OneToMany
  @OrderBy("hallName")
  private Set<Hall> halls;
  
  public Company(){}

  public Company(String name, String companyAddress)
  {
    super();
    this.name = name;
    this.companyAddress = companyAddress;
  }

  public void setIdCompany(int idCompany) {
    this.idCompany = idCompany;
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public String getCompanyAddress() {
    return companyAddress;
  }

  public void setCompanyAddress(String companyAddress) {
    this.companyAddress = companyAddress;
  }

  public Set<Hall> getHalls() {
    return halls;
  }

  public void setHalls(Set<Hall> halls) {
    this.halls = halls;
  }

  public int getIdCompany() {
    return idCompany;
  }

  
}
