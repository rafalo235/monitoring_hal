package com.hall.monitor.protocol;

import java.util.ArrayList;

public class SServerRequest implements UMessage
{
  private ArrayList<SRequest> requests;
  
  public SServerRequest(ArrayList<SRequest> requests)
  {
    super();
    this.requests = requests;
  }
  
  public char getRequestsSize() {
    return (char) requests.size();
  }
  
  public ArrayList<SRequest> getRequests() {
    return requests;
  }
  
}
