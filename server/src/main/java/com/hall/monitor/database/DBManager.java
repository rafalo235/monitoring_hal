package com.hall.monitor.database;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;

import com.hall.monitor.database.data.Company;
import com.hall.monitor.database.data.Concentrator;
import com.hall.monitor.database.data.ConcentratorConf;
import com.hall.monitor.database.data.Hall;
import com.hall.monitor.database.data.MonitorData;
import com.hall.monitor.database.data.Request;
import com.hall.monitor.database.data.RequestConf;
import com.hall.monitor.database.data.Sensor;
import com.hall.monitor.database.data.SensorData;
import com.hall.monitor.database.interfaces.IDBManager;
import com.hall.monitor.protocol.SConfigurationResponse;
import com.hall.monitor.protocol.SData;
import com.hall.monitor.protocol.SMonitorData;
import com.hall.monitor.protocol.SProtocol;
import com.hall.monitor.protocol.SRequest;
import com.hall.monitor.protocol.SSensorData;
import com.hall.monitor.protocol.SServerRequest;
import com.hall.monitor.protocol.SServerResponse;
import com.hall.monitor.protocol.UMessage;

public class DBManager implements IDBManager
{
  private Logger log = Logger.getLogger(DBManager.class.getSimpleName());
  
  /**
   * Zapisuje dane z czujnikow otrzymane od koncentratora
   * @param idPackage
   * @param concentratorId
   * @param monitor
   * @return
   */
  private boolean storeMonitorData(long idPackage, int concentratorId,
      SMonitorData monitor) {
    
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    // zapisz date pierwszej proby wyslania
    Date sendTime = new Date(monitor.getSendTime());
    
    // zapisz date otrzymania pakietu
    Date receiveTime = new Date();
    
    // pobierz ilosc czujnikow
    int sensorsAmount = monitor.getSensorsAmount();
    // znajdz w bazie koncentrator o danym id.
    Concentrator concentrator = getConcentrator(concentratorId);
    
    if (concentrator == null) {
      log.log(Level.SEVERE, "Concentrator wasn't found");
      session.getTransaction().rollback();
      session.close();
      return false;
    }
    else {
      // najpierw zapis naglowkow
      MonitorData monitorData = new MonitorData(idPackage, sendTime,
          receiveTime, sensorsAmount, concentrator);
      
      session.save(monitorData);
      // pobranie id dodanego naglowka
      int idMonitorData = ((BigInteger) session.createSQLQuery(
          "SELECT LAST_INSERT_ID()").uniqueResult()).intValue();
      monitorData.setIdMonitorData(idMonitorData);
      
      // pobranie czujnikow
      List<Sensor> sensors = getSensors(session, concentratorId);
      // dodanie otrzymanych danych
      for (SSensorData sensorData : monitor.getSensorsData()) {
        long idData = sensorData.getIdData();
        int sensorId = sensorData.getIdSensor();
        
        // poszukiwanie sensora
        Sensor sensor = null;
        for (Sensor sensorBuf : sensors) {
          if (sensorBuf.getIdSensor() == sensorId) {
            sensor = sensorBuf;
            break;
          }
        }
        if (sensor == null) {
          // czujnik nie znaleziony
          session.getTransaction().rollback();
          session.close();
          return false;
        }
        
        // dodanie danych
        Date timeStamp = new Date(sensorData.getTimeStamp());
        SData data = sensorData.getData();
        SensorData sdata = new SensorData(idData, sensor, timeStamp,
            data.getType(), String.valueOf(data.getValue()),
            sensorData.getSensorState(), sensorData.getDangerLevel(),
            monitorData);
        session.save(sdata);
        
      }
      // wszystko ok, wiec potwierdz transakcje
      session.getTransaction().commit();
      
    }
    session.close();
    return true;
  }
  
  /**
   * Zapisuje flage informujaca ze koncentrator zmienil konfiguracje.
   * @param confResponse
   * @return
   */
  private boolean storeConfigurationResponse(SConfigurationResponse confResponse) {
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    
    // pobierz id pakietu z konfiguracja, na ktory koncentratora odpowiada
    long idRequestPackage = confResponse.getIdRequestPackage();
    
    // zmien flage informujaca ze konfiguracja zostala poprawnie zmieniona
    
    ConcentratorConf conf = getConcentratorConfiguration(session,
        idRequestPackage);
    if (conf == null) {
      log.log(Level.SEVERE, "Configuration of concentrator wasn't found");
      return false;
    }
    
    conf.setChanged(true);
    session.update(conf);
    session.getTransaction().commit();
    return true;
    
  }
  
  private boolean storeServerRequest(long idPackage, int concentratorId, SServerRequest configRequest) {
    SessionFactory factory = HibernateUtil.getFactory();
    Session session = factory.openSession();
    session.beginTransaction();
    // zapisz date otrzymania pakietu
    Date receiveTime = new Date();
    

    // znajdz w bazie koncentrator o danym id.
    Concentrator concentrator = getConcentrator(concentratorId);
    
    if (concentrator == null) {
      log.log(Level.SEVERE, "Concentrator wasn't found");
      session.getTransaction().rollback();
      session.close();
      return false;
    }
    
    

    Request req = addRequest(session, idPackage, receiveTime, concentrator);
    if (addRequest(session, req, configRequest.getRequests()) == null){
      session.getTransaction().rollback();
      session.close();
      return false;
    }
    session.getTransaction().commit();
    session.close();
    return true;
  }


  /**
   * Zapisuje dane z protokolu w bazie
   * 
   * @return true, jesli wszystko ok, false, jesli nie
   */
  public boolean store(SProtocol protocol) {
    long idPackage = protocol.getIdPackage();
    int concentratorId = protocol.getIdConcentrator();
    UMessage umessage = protocol.getMessage();
    
    if (umessage instanceof SMonitorData) {
      // dane z czujnikow
      SMonitorData monitor = (SMonitorData) umessage;
      return storeMonitorData(idPackage, concentratorId, monitor);
    }
    else if (umessage instanceof SConfigurationResponse) {
      // potwierdzenie konfiguracji
      SConfigurationResponse configurationResponse = (SConfigurationResponse) umessage;
      return storeConfigurationResponse(configurationResponse);
    }
    else if (umessage instanceof SServerRequest) {
      // prosba o przeslanie konfiguracji
      SServerRequest configRequest = (SServerRequest) umessage;
      return storeServerRequest(idPackage, concentratorId, configRequest);
    }else if (umessage instanceof SServerResponse){
      // wyslane potwierdzenie odebrania pakietow
      
    }
    
    return false;
    
  }
  
  private List<RequestConf> addRequest(Session session, Request req, List<SRequest> reqs) {
    ArrayList<RequestConf> results = new ArrayList<RequestConf>();
    for (SRequest buf : reqs){

      RequestConf reqConfSensor = new RequestConf(req, buf.getConfigurationType());
      session.save(reqConfSensor);
      int idReq = ((BigInteger) session.createSQLQuery(
          "SELECT LAST_INSERT_ID()").uniqueResult()).intValue();
      reqConfSensor.setIdRequestConf(idReq);
      results.add(reqConfSensor);
    }
    

    return results;
  }
  
  private Request addRequest(Session session, long idPackage, Date receiveTime,
      Concentrator concentrator) {
    
    Request req = new Request(idPackage, receiveTime, concentrator, null);
    session.save(req);
    int idReq = ((BigInteger) session.createSQLQuery(
        "SELECT LAST_INSERT_ID()").uniqueResult()).intValue();
    req.setIdRequest(idReq);
    return req;
  }

  
  @SuppressWarnings("unchecked")
  private List<Sensor> getSensors(Session session, int idConcentrator) {
    Query query = session.createQuery("FROM Sensor WHERE idConcentrator = :id");
    query.setParameter("id", idConcentrator);
    return query.list();
  }
  
  @SuppressWarnings("unchecked")
  private List<Company> getCompanies(Session session) {
    Query query = session.createQuery("FROM Company");
    return query.list();
  }
  
  private Company getCompany(Session session, String name, String address) {
    Query query = session
        .createQuery("FROM Company WHERE companyName = :h1 AND companyAddress = :add");
    query.setParameter("h1", name);
    query.setParameter("add", address);
    
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
    Query query = session.createQuery("FROM Company WHERE idCompany = :id");
    query.setParameter("id", idCompany);
    
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
    Query query = session.createQuery("FROM Hall WHERE idCompany = :id");
    query.setParameter("id", idCompany);
    return query.list();
  }
  
  @SuppressWarnings("unchecked")
  private List<Hall> getHalls(Session session) {
    Query query = session.createQuery("FROM Hall");
    return query.list();
  }
  
  private Hall getHall(Session session, int idCompany, String hallName,
      String address) {
    Query query = session
        .createQuery("FROM Hall WHERE idCompany =:id AND hallName = :h1 AND hallAddress = :add");
    query.setParameter("id", idCompany);
    query.setParameter("h1", hallName);
    query.setParameter("add", address);
    
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
    Query query = session.createQuery("FROM Hall WHERE idHall = :id");
    query.setParameter("id", idHall);
    
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
    int idHall = ((BigInteger) session
        .createSQLQuery("SELECT LAST_INSERT_ID()").uniqueResult()).intValue();
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
    Query query = session
        .createQuery("FROM Concentrator WHERE idConcentrator = :id");
    query.setParameter("id", idConcentrator);
    
    @SuppressWarnings("rawtypes")
    List list = query.list();
    if (list.size() == 0) {
      return null;
    }
    else {
      return (Concentrator) list.iterator().next();
    }
    
  }
  
  private ConcentratorConf getConcentratorConfiguration(Session session,
      long idConcentratorConf) {
    Query query = session
        .createQuery("FROM Concentrator WHERE idConcentrator = :id");
    query.setParameter("id", idConcentratorConf);
    
    @SuppressWarnings("rawtypes")
    List list = query.list();
    if (list.size() == 0) {
      return null;
    }
    else {
      return (ConcentratorConf) list.iterator().next();
    }
  }
  
  // //////////////////////////////////////////////////////////
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
