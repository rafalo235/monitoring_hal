package com.hall.monitor.database.interfaces;

import java.util.List;

import com.hall.monitor.database.data.Company;
import com.hall.monitor.database.data.Concentrator;
import com.hall.monitor.database.data.ConcentratorConf;
import com.hall.monitor.database.data.Hall;
import com.hall.monitor.protocol.SConfiguration;
import com.hall.monitor.protocol.SProtocol;
import com.hall.monitor.protocol.SRequest;

public interface IDBManager
{
  public SConfiguration loadConcentratorConfiguration(int idConcentrator);
  public SConfiguration loadConcentratorConfiguration(int idConcentrator, List<SRequest> requests);
  
  public boolean storeConcentratorConfiguration(int concentratorId, ConcentratorConf conf);
  
  public Company addCompany(String name, String address);
  public Company getCompany(String name, String address);
  public List<Company> getCompanies();
  
  public Hall addHall(int idCompany, String hallName, String address);
  public Hall getHall(int idCompany, String hallName, String address);
  public List<Hall> getHalls(int idCompany);
  public List<Hall> getHalls();
  
  public Concentrator addConcentrator(int idHall, List<Integer> idCSensors);
  public Concentrator getConcentrator(int id);
  
  public boolean store(SProtocol protocol);
  
}
