package com.hall.monitor.rest;

import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;

import com.hall.monitor.database.DBManager;
import com.hall.monitor.database.data.Company;
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
      SProtocol test = getTest();
      b2 = ProtocolConverter.convertToBytes(test);
    }
    catch (ParserException e) {
      
      e.printStackTrace();
    }
    
    return Response.status(201).entity(b2).build();
    
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
  @Path("/test")
  @Produces(MediaType.TEXT_PLAIN)
  public Response testCreate() {
    try {
      DBManager db = new DBManager();
      Company comp = db.addCompany("Firma1", "Lodz");
      
      ResponseBuilder builder = Response.status(201);
      builder.entity("ID:" + comp.getIdCompany());
      Response res = builder.build();
      return res;
    }
    catch (Throwable e) {
      ResponseBuilder builder = Response.status(201);
      builder.entity("ID:");
      Response res = builder.build();
      return res;
    }
    
  }
}
