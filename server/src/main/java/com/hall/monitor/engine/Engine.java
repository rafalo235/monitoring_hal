package com.hall.monitor.engine;

import com.hall.monitor.database.DBManager;
import com.hall.monitor.protocol.SProtocol;

public class Engine
{
  private DBManager     db     = new DBManager();
  private static Engine engine = null;
  
  public static Engine getInstance() {
    if (engine == null) {
      engine = new Engine();
    }
    return engine;
  }
  
  public SProtocol receiveProtocol(SProtocol protocol) {
    synchronized (this) {
      db.store(protocol);
    }
    return null;
  }
}
