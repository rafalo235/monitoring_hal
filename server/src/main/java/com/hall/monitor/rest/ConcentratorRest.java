package com.hall.monitor.rest;


import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import com.hall.monitor.protocol.EMessageType;
import com.hall.monitor.protocol.EReceiveStatus;
import com.hall.monitor.protocol.SConfiguration;
import com.hall.monitor.protocol.SConfigurationValue;
import com.hall.monitor.protocol.SProtocol;
import com.hall.monitor.protocol.SServerResponse;
import com.hall.monitor.rest.ProtocolConverter.ParserException;



@Path("/concentrator")
public class ConcentratorRest {

  Logger log = Logger.getLogger(ConcentratorRest.class.getSimpleName());
 
	@POST
	@Path("/post")
	@Consumes(MediaType.APPLICATION_OCTET_STREAM)
	@Produces(MediaType.APPLICATION_OCTET_STREAM)
	public Response receiveData(byte bytes[]) {

	  byte b2[] = null;
	  try {
      SProtocol protocol = ProtocolConverter.convertToProtocol(bytes);
      log.log(Level.INFO, "Protocol data received: id=" + protocol.getIdPackage());
      SProtocol test = getTest();
      b2 = ProtocolConverter.convertToBytes(test);
    }
    catch (ParserException e) {

      e.printStackTrace();
    }

		return Response.status(201).entity(b2).build();
		
	}
	
	
	private SProtocol getTest(){
	  
	  ArrayList<SConfigurationValue> configurations= new ArrayList<SConfigurationValue>();
    SConfiguration configuration = new SConfiguration(configurations);
    SServerResponse res = new SServerResponse(EReceiveStatus.OK, 9, configuration);
	  return new SProtocol((char)1, 0L, (char)10, (char) 0, 2, EMessageType.SERVER_MONITOR_RESPONSE, res);
	}
}
