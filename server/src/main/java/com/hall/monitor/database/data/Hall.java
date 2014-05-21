package com.hall.monitor.database.data;

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

import org.hibernate.annotations.OrderBy;

@Entity
@Table(name = "Halls")
public class Hall
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idHall")
  private int               idHall;
  
  @Column(nullable = false, name = "hallName")
  private String            hallName;
  
  @Column(nullable = false, name = "address")
  private String            address;
  
  @ManyToOne
  @JoinColumn(name = "idCompanay")
  private Company           company;
  
  @OneToMany(mappedBy = "hall")
  @OrderBy(clause = "idConcentator")
  private Set<Concentrator> concentrators;
  
  public Hall()
  {
  }
  
  public Hall(String hallName, String address, Company company)
  {
    super();
    this.hallName = hallName;
    this.address = address;
    this.company = company;
  }
  
  public void setIdHall(int idHall) {
    this.idHall = idHall;
  }
  
  public String getHallName() {
    return hallName;
  }
  
  public void setHallName(String hallName) {
    this.hallName = hallName;
  }
  
  public String getAddress() {
    return address;
  }
  
  public void setAddress(String address) {
    this.address = address;
  }
  
  public Company getCompany() {
    return company;
  }
  
  public void setCompany(Company company) {
    this.company = company;
  }
  
  public Set<Concentrator> getConcentrators() {
    return concentrators;
  }
  
  public void setConcentrators(Set<Concentrator> concentrators) {
    this.concentrators = concentrators;
  }
  
  public int getIdHall() {
    return idHall;
  }
  
}
