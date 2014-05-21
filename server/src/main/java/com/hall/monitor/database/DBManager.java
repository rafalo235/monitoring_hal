package com.hall.monitor.database;

import java.math.BigInteger;
import java.util.Date;
import java.util.Iterator;
import java.util.List;

import org.hibernate.SQLQuery;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;

import com.hall.monitor.database.data.Company;
import com.hall.monitor.database.data.Concentrator;
import com.hall.monitor.database.data.Hall;
import com.hall.monitor.database.data.MonitorData;
import com.hall.monitor.database.data.Sensor;
import com.hall.monitor.database.data.SensorData;
import com.hall.monitor.database.interfaces.IDBManager;
import com.hall.monitor.protocol.SData;
import com.hall.monitor.protocol.SMonitorData;
import com.hall.monitor.protocol.SProtocol;
import com.hall.monitor.protocol.SSensorData;
import com.hall.monitor.protocol.UMessage;

public class DBManager implements IDBManager
{
  
  public boolean store(SProtocol protocol) {
    UMessage umessage = protocol.getMessage();
    if (!(umessage instanceof SMonitorData)) {
      return false;
    }
    
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    Transaction transation = session.beginTransaction();
    
    SMonitorData monitor = (SMonitorData) umessage;

    Date sendTime = new Date(monitor.getSendTime());
    long idPackage = protocol.getIdPackage();
    
    Date receiveTime = new Date();
    
    int sensorsAmount = monitor.getSensorsAmount();
    
    int concentratorId = protocol.getIdConcentrator();
    Concentrator concentrator = getConcentrator(concentratorId);
    
    if (concentrator != null) {
      
      MonitorData monitorData = new MonitorData(idPackage, sendTime,
          receiveTime, sensorsAmount, concentrator);
      
      session.save(monitorData);
      BigInteger a = ((BigInteger) session.createSQLQuery(
          "SELECT LAST_INSERT_ID()").uniqueResult());
      int idMonitorData = ((BigInteger) session.createSQLQuery(
          "SELECT LAST_INSERT_ID()").uniqueResult()).intValue();
      monitorData.setIdMonitorData(idMonitorData);
      
      List<Sensor> sensors = getSensors(session, concentratorId);
      
      for (SSensorData sensorData : monitor.getSensorsData()){
        long idData = sensorData.getIdData();
        int sensorId = sensorData.getIdSensor();
        
        Sensor sensor = null;
        for (Sensor sensorBuf : sensors){
          if (sensorBuf.getIdSensor() == sensorId){
            sensor = sensorBuf;
            break;
          }
        }
        if (sensor == null){
          session.close();
          return false;
        }
        
        Date timeStamp = new Date(sensorData.getTimeStamp());
        SData data = sensorData.getData();
        SensorData sdata = new SensorData(
            idData, sensor, timeStamp, 
            data.getType(), String.valueOf(data.getValue()), 
            sensorData.getSensorState(), sensorData.getDangerLevel(), monitorData);
        session.save(sdata);

      }

      transation.commit();
      
    }
    session.close();
    
    return true;
    
  }
  
  @SuppressWarnings("unchecked")
  private List<Sensor> getSensors(Session session, int idConcentrator){
    SQLQuery query = session.createSQLQuery("SELECT * FROM Sensors WHERE idConcentrator = ?");
    query.setInteger(1, idConcentrator);
    return query.list();
  }
  @SuppressWarnings("unchecked")
  private List<Company> getCompanies(Session session) {
    SQLQuery query = session.createSQLQuery("SELECT * FROM Companies");
    
    return query.list();
  }
  
  private Company getCompany(Session session, String name, String address) {
    SQLQuery query = session
        .createSQLQuery("SELECT * FROM Companies WHERE name = ? AND companyAddress = ?");
    query.setString(1, name);
    query.setString(2, address);
    @SuppressWarnings("unchecked")
    Iterator<Company> it = query.list().iterator();
    if (it.hasNext()) {
      return it.next();
    }
    else {
      return null;
    }
  }
  
  private Company getCompany(Session session, int idCompany) {
    SQLQuery query = session
        .createSQLQuery("SELECT * FROM Companies WHERE idCompany = ?");
    query.setInteger(1, idCompany);
    @SuppressWarnings("unchecked")
    Iterator<Company> it = query.list().iterator();
    if (it.hasNext()) {
      return it.next();
    }
    else {
      return null;
    }
  }
  
  private Company addCompany(Session session, String name, String address) {
    Company comp = new Company(name, address);
    session.save(comp);
    int idCompany = ((BigInteger) session.createSQLQuery(
        "SELECT LAST_INSERT_ID()").uniqueResult()).intValue();
    comp.setIdCompany(idCompany);
    return comp;
  }

  
  @SuppressWarnings("unchecked")
  private List<Hall> getHalls(Session session, int idCompany) {
    SQLQuery query = session
        .createSQLQuery("SELECT * FROM Halls WHERE idCompany =?");
    query.setInteger(1, idCompany);
    return query.list();
  }
  
  @SuppressWarnings("unchecked")
  private List<Hall> getHalls(Session session) {
    SQLQuery query = session.createSQLQuery("SELECT * FROM Halls");
    
    return query.list();
  }
  
  private Hall getHall(Session session, int idCompany, String hallName,
      String address) {
    SQLQuery query = session
        .createSQLQuery("SELECT * FROM Halls WHERE idCompany = ? AND hallName = ? AND address =?");
    query.setInteger(1, idCompany);
    query.setString(2, hallName);
    query.setString(3, address);
    
    @SuppressWarnings("rawtypes")
    List list = query.list();
    if (list.size() == 0) {
      return null;
    }
    else {
      return (Hall) list.iterator().next();
    }
  }
  
  private Hall getHall(Session session, int idHall) {
    SQLQuery query = session
        .createSQLQuery("SELECT * FROM Halls WHERE idHall = ? ");
    query.setInteger(1, idHall);
    
    @SuppressWarnings("rawtypes")
    List list = query.list();
    if (list.size() == 0) {
      return null;
    }
    else {
      return (Hall) list.iterator().next();
    }
  }
  
  private Hall addHall(Session session, int idCompany, String hallName,
      String address) {
    Company company = getCompany(session, idCompany);
    if (company == null) {
      return null;
    }
    Hall hall = new Hall(hallName, address, company);
    session.save(hall);
    int idHall = ((BigInteger) session.createSQLQuery(
        "SELECT LAST_INSERT_ID()").uniqueResult()).intValue();
    hall.setIdHall(idHall);
    return hall;
  }
  
  private Concentrator addConcentrator(Session session, int idHall,
      List<Integer> idCSensors) {
    Hall hall = getHall(session, idHall);
    if (hall == null) {
      return null;
    }
    Concentrator concentrator = new Concentrator(hall);
    session.save(concentrator);
    int idConcentrator = ((BigInteger) session.createSQLQuery(
        "SELECT LAST_INSERT_ID()").uniqueResult()).intValue();
    concentrator.setIdConcentrator(idConcentrator);
    
    for (Integer idConcentratorSensor : idCSensors) {
      Sensor sensor = new Sensor(concentrator, idConcentratorSensor);
      session.save(sensor);
    }
    return concentrator;
  }
  
  private Concentrator getConcentrator(Session session, int idConcentrator) {
    SQLQuery query = session
        .createSQLQuery("SELECT * FROM Concentrators WHERE idConcentrator = ? ");
    query.setInteger(1, idConcentrator);
    
    @SuppressWarnings("rawtypes")
    List list = query.list();
    if (list.size() == 0) {
      return null;
    }
    else {
      return (Concentrator) list.iterator().next();
    }
    
  }
  
  @Override
  public Company addCompany(String name, String address) {
    
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    
    Company company = addCompany(session, name, address);
    session.getTransaction().commit();
    session.close();
    
    return company;
  }
  
  @Override
  public Company getCompany(String name, String address) {
    
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    
    Company company = getCompany(session, name, address);
    session.getTransaction().commit();
    session.close();
    return company;
  }
  
  @Override
  public List<Company> getCompanies() {
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    
    List<Company> companies = getCompanies(session);
    session.getTransaction().commit();
    session.close();
    return companies;
  }
  
  @Override
  public Hall addHall(int idCompany, String hallName, String address) {
    
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    
    Hall hall = addHall(session, idCompany, hallName, address);
    session.getTransaction().commit();
    session.close();
    return hall;
  }
  
  @Override
  public Hall getHall(int idCompany, String hallName, String address) {
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    
    Hall hall = getHall(session, idCompany, hallName, address);
    session.getTransaction().commit();
    session.close();
    return hall;
  }
  
  @Override
  public List<Hall> getHalls(int idCompany) {
    
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    
    List<Hall> halls = getHalls(session, idCompany);
    session.getTransaction().commit();
    session.close();
    return halls;
  }
  
  @Override
  public List<Hall> getHalls() {
    List<Hall> halls = null;
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    
    halls = getHalls(session);
    session.getTransaction().commit();
    session.close();
    return halls;
  }
  
  @Override
  public Concentrator addConcentrator(int idHall, List<Integer> idCSensors) {
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    
    Concentrator conc = addConcentrator(session, idHall, idCSensors);
    session.getTransaction().commit();
    session.close();
    return conc;
  }
  
  @Override
  public Concentrator getConcentrator(int id) {
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    
    Concentrator conc = getConcentrator(session, id);
    session.getTransaction().commit();
    session.close();
    return conc;
  }
  
}
