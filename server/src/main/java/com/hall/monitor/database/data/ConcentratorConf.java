package com.hall.monitor.database.data;

import java.util.Date;
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

import org.hibernate.annotations.ForeignKey;

@Entity
@Table(name = "ConcentratorConfs")
public class ConcentratorConf
{
  @Id
  @GeneratedValue(strategy=GenerationType.AUTO)
  @Column
  private int idConcentratorConf;
  
  @ManyToOne
  @JoinColumn(name = "idConcentrator")
  @ForeignKey(name="FK_concentrator1")
  private Concentrator concentrator;
  
  @OneToMany
  private Set<SensorConf> sensorConf;
  
  
  @ManyToOne
  @JoinColumn(name = "idUser")
  @ForeignKey(name="FK_user1")
  private User user;
  
  @Column
  private boolean changed;
  
  @Column
  private Date timeStamp;
  
  public ConcentratorConf(){
    
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

  public Concentrator getConcentrator() {
    return concentrator;
  }

  public void setConcentrator(Concentrator concentrator) {
    this.concentrator = concentrator;
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
