package com.hall.monitor.database.data;

import java.util.Set;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.OneToMany;
import javax.persistence.Table;

@Entity
@Table(name = "Companies")
public class Company
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idCompany")
  private int       idCompany;
  
  @Column(nullable = false, name = "companyName")
  private String    companyName;
  
  @Column(nullable = false, name = "companyAddress")
  private String    companyAddress;
  
  @OneToMany(mappedBy = "company", fetch = FetchType.EAGER)
  private Set<Hall> halls;
  
  public Company()
  {
  }
  
  public Company(String companyName, String companyAddress)
  {
    super();
    this.companyName = companyName;
    this.companyAddress = companyAddress;
  }
  
  public String getCompanyName() {
    return companyName;
  }
  
  public void setCompanyName(String companyName) {
    this.companyName = companyName;
  }
  
  public void setIdCompany(int idCompany) {
    this.idCompany = idCompany;
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
