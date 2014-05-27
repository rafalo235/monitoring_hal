package com.hall.monitor.rest;

import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
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
import com.hall.monitor.database.data.Hall;
import com.hall.monitor.engine.Engine;
import com.hall.monitor.protocol.EConfigurationType;
import com.hall.monitor.protocol.EMessageType;
import com.hall.monitor.protocol.EReceiveStatus;
import com.hall.monitor.protocol.EValueType;
import com.hall.monitor.protocol.SConfiguration;
import com.hall.monitor.protocol.SConfigurationValue;
import com.hall.monitor.protocol.SData;
import com.hall.monitor.protocol.SProtocol;
import com.hall.monitor.protocol.SServerResponse;
import com.hall.monitor.rest.ProtocolConverter.ParserException;

@Path("/concentrator")
public class ConcentratorRest
{
  
  Logger log = Logger.getLogger(ConcentratorRest.class.getSimpleName());
  
  @POST
  @Path("/post")
  @Consumes(MediaType.APPLICATION_OCTET_STREAM)
  @Produces(MediaType.APPLICATION_OCTET_STREAM)
  public Response receiveData(byte bytes[]) {
    
    byte b2[] = null;
    try {
      SProtocol protocol = ProtocolConverter.convertToProtocol(bytes);
      log.log(Level.INFO,
          "Protocol data received: id=" + protocol.getIdPackage());
      
      Engine.getInstance().receiveProtocol(protocol);
      SProtocol test = getTest();
      b2 = ProtocolConverter.convertToBytes(test);
    }
    catch (ParserException e) {
      
      e.printStackTrace();
    }
    
    return Response.status(201).entity(b2).build();
    
  }
  
  // ////////////////////////////////////////////////////////////////////////////////////////////////////
  private Response sendError(){
    ResponseBuilder builder = Response.status(201);
    builder.entity("Blad");
    Response res = builder.build();
    return res;
  }
  private SProtocol getTest() {
    
    ArrayList<SConfigurationValue> configurations = new ArrayList<SConfigurationValue>();
    SData data1 = new SData(EValueType.INT_32, 100);
    SConfigurationValue val1 = new SConfigurationValue((byte) 1,
        EConfigurationType.DANGER_LEVEL, data1);
    configurations.add(val1);
    
    SData data2 = new SData(EValueType.INT_32, 200);
    SConfigurationValue val2 = new SConfigurationValue((byte) 2,
        EConfigurationType.DANGER_LEVEL, data2);
    configurations.add(val2);
    
    SData data3 = new SData(EValueType.UINT_32, 200L);
    SConfigurationValue val3 = new SConfigurationValue((byte) 1,
        EConfigurationType.DANGER_LEVEL, data3);
    configurations.add(val3);
    
    SConfiguration configuration = new SConfiguration(configurations);
    SServerResponse res = new SServerResponse(EReceiveStatus.OK, 9,
        configuration);
    return new SProtocol((char) 1, 0L, (char) 10, (char) 0, 2,
        EMessageType.SERVER_MONITOR_RESPONSE, res);
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
      String str = company.getIdCompany() + " : " + company.getCompanyName() + " : "
          + company.getCompanyAddress();
      
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
  
  /////////////////////////// concentrator
  @GET
  @Path("/test/add_concentrator")
  @Produces(MediaType.TEXT_PLAIN)
  public Response testAddConcentrator() {
    try {
      DBManager db = new DBManager();
      Company company = db.getCompany("Firma1", "Lodz");
      Hall hall = db.getHall(company.getIdCompany(), "HallName1", "HallAddresss1");
      List<Integer> idSensors = new ArrayList<Integer>();
      idSensors.add(1);
      Concentrator concentrator = db.addConcentrator(hall.getIdHall(), idSensors);
      
      ResponseBuilder builder = Response.status(201);
      builder.entity("ID CONCENTRATOR:" + concentrator.getIdConcentrator());
      Response res = builder.build();
      return res;
    }
    catch (Throwable e) {
      return sendError();
    }
  }
  @GET
  @Path("/test/add_test")
  @Produces(MediaType.TEXT_PLAIN)
  public Response testAddTest() {
    testAddCompany();
    testAddHall();
    testAddConcentrator();
    testAddConcentrator();
    return testAddConcentrator();
  }
  
}
