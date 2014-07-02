package com.hall.monitor.rest;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.logging.Logger;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;

import com.hall.monitor.database.DBManager;
import com.hall.monitor.database.data.Company;
import com.hall.monitor.database.data.Concentrator;
import com.hall.monitor.database.data.ConcentratorConf;
import com.hall.monitor.database.data.Hall;
import com.hall.monitor.database.data.Sensor;
import com.hall.monitor.database.data.SensorConf;
import com.hall.monitor.database.data.User;
import com.hall.monitor.engine.Engine;
import com.hall.monitor.protocol.EConfigurationType;
import com.hall.monitor.protocol.EValueType;
import com.hall.monitor.protocol.SConfiguration;
import com.hall.monitor.protocol.SConfigurationValue;

@Path("/concentrator")
public class ConcentratorRest
{
  
  Logger         log    = Logger.getLogger(ConcentratorRest.class
                            .getSimpleName());
  private Engine engine = new Engine();
  
  @POST
  @Path("/post")
  @Consumes(MediaType.APPLICATION_OCTET_STREAM)
  @Produces(MediaType.APPLICATION_OCTET_STREAM)
  public Response receiveData(byte bytes[]) {

    byte response[] = engine.onReceiveData(bytes);
    if (response == null) {
      // odpowiedz na potwierdzenie konfiguracja
      return Response.status(201).build();
    }

    return Response.status(201).entity(response).build();
    
  }
  
  // ////////////////////////////////////////////////////////////////////////////////////////////////////
  private Response sendError() {
    ResponseBuilder builder = Response.status(201);
    builder.entity("Blad");
    Response res = builder.build();
    return res;
  }
 
  
  @GET
  @Path("/test/get_configuration")
  @Produces(MediaType.TEXT_PLAIN)
  public Response testGetConfiguration()
  {
    DBManager db = new DBManager();
    SConfiguration conf = db.loadConcentratorConfiguration(1);
    StringBuilder str = new StringBuilder();
    str.append("Configuration size: "+ (int)conf.getConfigurationSize());
    for(SConfigurationValue value : conf.getConfigurations()){
      str.append("\nid Sensor: ");
      str.append((int)value.getIdSensor());
      str.append(" type: ");
      str.append(value.getConfigurationType().toString());
      str.append(" data: ");
      str.append(value.getData().getValue().toString());

    }
    return Response.status(201).entity(str.toString()).build();
  }
  @GET
  @Path("/test/add_company")
  @Produces(MediaType.TEXT_PLAIN)
  public Response testAddCompany() {
    try {
      DBManager db = new DBManager();
      Company comp = db.addCompany("Firma1", "Lodz");
      
      ResponseBuilder builder = Response.status(201);
      builder.entity("ID:" + comp.getIdCompany());
      Response res = builder.build();
      return res;
    }
    catch (Throwable e) {
      return sendError();
    }
    
  }
  
  private String companiesToStr(List<Company> companies) {
    StringBuilder str = new StringBuilder();
    
    for (int i = 0; i < companies.size(); ++i) {
      Object obj = companies.get(i);
      Company c = (Company) obj;
      str.append(i);
      str.append(": ");
      str.append(c.getCompanyName());
      str.append(": ");
      str.append(c.getCompanyAddress());
      str.append("\n");
    }
    return str.toString();
  }
  
  @GET
  @Path("/test/get_companies2")
  @Produces(MediaType.TEXT_PLAIN)
  public Response testGetCompanies2(@QueryParam("name") String companyName,
      @QueryParam("address") String address) {
    try {
      DBManager db = new DBManager();
      Company company = db.getCompany(companyName, address);
      String str = company.getIdCompany() + " : " + company.getCompanyName()
          + " : " + company.getCompanyAddress();
      
      ResponseBuilder builder = Response.status(201);
      builder.entity(str.toString());
      Response res = builder.build();
      return res;
    }
    catch (Throwable e) {
      return sendError();
    }
  }
  
  @GET
  @Path("/test/get_companies")
  @Produces(MediaType.TEXT_PLAIN)
  public Response testGetCompanies() {
    try {
      DBManager db = new DBManager();
      List<Company> companies = db.getCompanies();
      String str = companiesToStr(companies);
      
      ResponseBuilder builder = Response.status(201);
      builder.entity(str.toString());
      Response res = builder.build();
      return res;
    }
    catch (Throwable e) {
      return sendError();
    }
  }
  
  // /////////////////////// hall
  @GET
  @Path("/test/add_hall")
  @Produces(MediaType.TEXT_PLAIN)
  public Response testAddHall() {
    try {
      DBManager db = new DBManager();
      Company company = db.getCompany("Firma1", "Lodz");
      Hall hall = db.addHall(company.getIdCompany(), "HallName1",
          "HallAddresss1");
      
      ResponseBuilder builder = Response.status(201);
      builder.entity("ID HALL:" + hall.getIdHall());
      Response res = builder.build();
      return res;
    }
    catch (Throwable e) {
      return sendError();
    }
  }
  
  @GET
  @Path("/test/get_halls")
  @Produces(MediaType.TEXT_PLAIN)
  public Response testGetHalls() {
    try {
      DBManager db = new DBManager();
      List<Hall> halls = db.getHalls();
      StringBuilder str = new StringBuilder();
      
      for (int i = 0; i < halls.size(); ++i) {
        Object obj = halls.get(i);
        Hall h = (Hall) obj;
        str.append(i);
        str.append(": ");
        str.append(h.getHallName());
        str.append(": ");
        str.append(h.getHallAddress());
        str.append("\n");
      }
      
      ResponseBuilder builder = Response.status(201);
      builder.entity(str.toString());
      Response res = builder.build();
      return res;
    }
    catch (Throwable e) {
      return sendError();
    }
  }
  
  // ///////////////////////// concentrator
  @GET
  @Path("/test/add_concentrator")
  @Produces(MediaType.TEXT_PLAIN)
  public Response testAddConcentrator() {
    return addConcentrator(1);
  }
  
  private Response addConcentrator(int size) {
    try {
      DBManager db = new DBManager();
      Company company = db.getCompany("Firma1", "Lodz");
      Hall hall = db.getHall(company.getIdCompany(), "HallName1",
          "HallAddresss1");
      List<Integer> idSensors = new ArrayList<Integer>();
      for (int i = 0; i < size; ++i) {
        idSensors.add(i);
      }
      Concentrator concentrator = db.addConcentrator(hall.getIdHall(),
          idSensors);
      
      ResponseBuilder builder = Response.status(201);
      builder.entity("ID CONCENTRATOR:" + concentrator.getIdConcentrator());
      Response res = builder.build();
      return res;
    }
    catch (Throwable e) {
      return sendError();
    }
  }
  
  private void addUsers(int size) {
    DBManager db = new DBManager();
    for (int i = 0; i < size; ++i) {
      String firstName = "firstName" + i;
      String lastName = "lastName" + i;
      String login = "login" + i;
      String pass = "pass" + i;
      db.addUser(firstName, lastName, login, pass);
    }
  }
  
  private void addConfigure() {
    try {
      DBManager db = new DBManager();
      Company company = db.getCompany("Firma1", "Lodz");
      Hall hall = db.getHall(company.getIdCompany(), "HallName1",
          "HallAddresss1");
      List<Concentrator> concs = db.getConcentrators(hall.getIdHall());
      Concentrator concentrator = concs.iterator().next();
      User user = db.getUser("login0", "pass0");
      
      Set<SensorConf> sensorCon = new HashSet<SensorConf>();
      Set<Sensor> sensors = concentrator.getSensors();
      Sensor sensor = null;
      for(Sensor s : sensors)
      {
        if (s.getIdConcentratorSensor()== 0)
        {
          sensor = s;
          break;
        }
      }
      Date timeStamp = new Date();
      boolean changed = false;
      ConcentratorConf conf1 = new ConcentratorConf(concentrator, sensorCon,
          user, changed, timeStamp);

      sensorCon.add(new SensorConf(conf1, sensor, EConfigurationType.ALARM_LEVEL,
          EValueType.INT_32, String.valueOf(40)));
      sensorCon.add(new SensorConf(conf1, sensor, EConfigurationType.DANGER_LEVEL,
          EValueType.INT_32, String.valueOf(20)));
      
      db.storeConcentratorConfiguration(concentrator.getIdConcentrator(), conf1);
    }
    catch (Throwable e) {
      e.printStackTrace();
    }
  }
  
  @GET
  @Path("/test/add_test")
  @Produces(MediaType.TEXT_PLAIN)
  public Response testAddTest() {
    testAddCompany();
    testAddHall();
    addUsers(3);
    Response res = addConcentrator(4);
    addConfigure();
    return res;
  }
  
}
