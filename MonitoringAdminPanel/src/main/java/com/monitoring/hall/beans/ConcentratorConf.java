package com.monitoring.hall.beans;

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

@Entity
@Table(name = "ConcentratorConfs")
public class ConcentratorConf
{
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  @Column(name = "idConcentratorConf")
  private int             idConcentratorConf;
  
  @Column(name = "idPackage")
  private long idPackage;
  
  @ManyToOne
  @JoinColumn(name = "idConcentrator")
  private Concentrator    concentrator;
  
  @OneToMany(mappedBy = "concentratorConf", fetch = FetchType.EAGER)
  private Set<SensorConf> sensorConf;
  
  @ManyToOne
  @JoinColumn(name = "idUser")
  private User            user;
  
  @Column(name = "changed")
  private boolean         changed;
  
  @Column(name = "timeStamp")
  private Date            timeStamp;
  
  public ConcentratorConf()
  {
    
  }
  
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
  
  public void setIdConcentratorConf(int idConcentratorConf) {
    this.idConcentratorConf = idConcentratorConf;
  }

  public Concentrator getConcentrator() {
    return concentrator;
  }
  
  public void setConcentrator(Concentrator concentrator) {
    this.concentrator = concentrator;
  }
  
  public long getIdPackage() {
    return idPackage;
  }

  public void setIdPackage(long idPackage) {
    this.idPackage = idPackage;
  }

  public Set<SensorConf> getSensorConf() {
    return sensorConf;
  }
  
  public void setSensorConf(Set<SensorConf> sensorConf) {
    this.sensorConf = sensorConf;
  }
  
  public User getUser() {
    return user;
  }
  
  public void setUser(User user) {
    this.user = user;
  }
  
  public boolean isChanged() {
    return changed;
  }
  
  public void setChanged(boolean changed) {
    this.changed = changed;
  }
  
  public Date getTimeStamp() {
    return timeStamp;
  }
  
  public void setTimeStamp(Date timeStamp) {
    this.timeStamp = timeStamp;
  }
  
  public int getIdConcentratorConf() {
    return idConcentratorConf;
  }
  
}
