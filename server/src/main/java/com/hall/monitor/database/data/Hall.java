package com.hall.monitor.database.data;

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
  private String            hallAddress;
  
  @ManyToOne
  @JoinColumn(name = "idCompany")
  private Company           company;
  
  @OneToMany(mappedBy = "hall", fetch = FetchType.EAGER)
  private Set<Concentrator> concentrators;
  
  public Hall()
  {
  }
  
  public Hall(String hallName, String hallAddress, Company company)
  {
    super();
    this.hallName = hallName;
    this.hallAddress = hallAddress;
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
  
  public String getHallAddress() {
    return hallAddress;
  }
  
  public void setHallAddress(String hallAddress) {
    this.hallAddress = hallAddress;
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
