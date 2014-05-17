package com.hall.monitor.protocol;

import java.util.ArrayList;

public class SServerRequest implements UMessage
{
  // uint8_t requestsSize;
  private char                requestsSize;
  
  // SRequest* requests;
  private ArrayList<SRequest> requests;
  
  public static final int     HEADER_SIZE = (EValueType.UINT_8.getTypeSizeBits()) / 8;
  
  public SServerRequest(ArrayList<SRequest> requests)
  {
    super();
    this.requests = requests;
    this.requestsSize = (char) requests.size();
  }
  
  public char getRequestsSize() {
    return requestsSize;
  }
  
  public ArrayList<SRequest> getRequests() {
    return requests;
  }
  
  @Override
  public int getSize() {
    int size = 0;
    for (SRequest req : requests) {
      size += req.getSize();
    }
    return HEADER_SIZE + size;
  }
  
}
