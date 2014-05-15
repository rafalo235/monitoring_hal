package com.hall.monitor.rest;


import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;



@Path("/concentrator")
public class ConcentratorRest {

	

	@POST
	@Path("/post")
	@Consumes(MediaType.APPLICATION_OCTET_STREAM)
	@Produces(MediaType.APPLICATION_OCTET_STREAM)
	public Response createTrackInJSON(byte bytes[]) {

		String result = "working";
		byte b2[] = result.getBytes();
		return Response.status(201).entity(b2).build();
		
	}
	
}
