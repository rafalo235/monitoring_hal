package com.hall.monitor;

import javax.ws.rs.core.MediaType;

import org.junit.Test;

import com.sun.jersey.api.client.Client;
import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.WebResource;


public class ConcentratorRestTest {

	@Test
	public void test(){
		
		try {

			Client client = Client.create();

			WebResource webResource = client
					.resource("http://localhost:8080/HallMonitorServer/rest/concentrator");

    
      ClientResponse response = webResource.accept(MediaType.TEXT_PLAIN)
          .get(ClientResponse.class);

      if (response.getStatus() != 200) {
        throw new RuntimeException("Failed : HTTP error code : "
            + response.getStatus());
      }

      String output = response.getEntity(String.class);

      System.out.println("Output from Server .... \n");
      System.out.println(output);
		} catch (Exception e) {

			e.printStackTrace();

		}

	}


}
