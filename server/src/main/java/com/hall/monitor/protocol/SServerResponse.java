package com.hall.monitor.protocol;

import java.util.ArrayList;

public class SServerResponse implements UMessage
{
  private ArrayList<SRequest> requests;
  
  public SServerResponse(ArrayList<SRequest> requests)
  {
    super();
    this.requests = requests;
  }
  
  public ArrayList<SRequest> getRequests() {
    return requests;
  }
  
}
