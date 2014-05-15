package com.hall.monitor.rest;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;



@Path("/concentrator")
public class ConcentratorRest {

	

	@POST
	@Path("/post")
	@Consumes(MediaType.APPLICATION_JSON)
	public Response createTrackInJSON(byte bytes[]) {

		String result = "working";
		return Response.status(201).entity(result).build();
		
	}
	
}
