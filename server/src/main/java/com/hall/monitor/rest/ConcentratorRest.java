package com.hall.monitor.rest;


import java.util.logging.Level;
import java.util.logging.Logger;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import com.hall.monitor.protocol.SProtocol;
import com.hall.monitor.rest.ProtocolConverter.ParserException;



@Path("/concentrator")
public class ConcentratorRest {

  Logger log = Logger.getLogger(ConcentratorRest.class.getSimpleName());
 
	@POST
	@Path("/post")
	@Consumes(MediaType.APPLICATION_OCTET_STREAM)
	@Produces(MediaType.APPLICATION_OCTET_STREAM)
	public Response receiveData(byte bytes[]) {

	  try {
      SProtocol protocol = ProtocolConverter.convertToProtocol(bytes);
      log.log(Level.SEVERE, "" + protocol.getVersion());
    }
    catch (ParserException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
		String result = "working";
		byte b2[] = result.getBytes();
		return Response.status(201).entity(b2).build();
		
	}
	
}
