package com.monitoring.hall.persistence;

import java.util.List;

import com.monitoring.hall.beans.Company;
import com.monitoring.hall.beans.Concentrator;
import com.monitoring.hall.beans.ConcentratorConf;
import com.monitoring.hall.beans.Hall;
import com.monitoring.hall.beans.MonitorData;
import com.monitoring.hall.beans.SensorConf;
import com.monitoring.hall.beans.SensorData;

public interface PersistenceDAO {
	
	public void addCompany(Company company);
	public List<Company> listCompany();
	public void removeCompany(Integer id);
	
	public void addHall(Hall hall);
	public List<Hall> listHalls();
	public void removeHall(Integer id);
	public List<Hall> getHalls(int idCompany);
	
	public void addConcentrator(Concentrator concentrator);
	public List<Concentrator> listConcentrators();
	public void removeConcentrator(Integer id);
	public List<Concentrator> getConcentrators(int idHall);
	public List<MonitorData> getMonitorDatas(int idConcentrator);
	public List<SensorData> getSensorDatas(int idMonitorData);
	public List<SensorData> listSensorDatas();
	
	public ConcentratorConf getConcentratorConf(int idConcentrator);
	public void setConcentratorConf(ConcentratorConf concentratorConf);
	
	public SensorConf getSensorConf(int sensorConfId);
	public void setSensorConf(SensorConf sensorConf);
}
