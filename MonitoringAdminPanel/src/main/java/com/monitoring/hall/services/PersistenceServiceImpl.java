package com.monitoring.hall.services;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.monitoring.hall.beans.Company;
import com.monitoring.hall.beans.Concentrator;
import com.monitoring.hall.beans.Hall;
import com.monitoring.hall.beans.MonitorData;
import com.monitoring.hall.beans.SensorData;
import com.monitoring.hall.persistence.PersistenceDAO;

@Service
@Transactional
public class PersistenceServiceImpl implements PersistenceService {

	@Autowired
	PersistenceDAO persistence;
	
	@Override
	public void addCompany(Company company) {
		persistence.addCompany(company);

	}

	@Override
	public List<Company> listCompany() {
		
		return persistence.listCompany();
	}

	@Override
	public void removeCompany(Integer id) {
		persistence.removeCompany(id);

	}

	@Override
	public void addHall(Hall hall) {
		persistence.addHall(hall);
		
	}

	@Override
	public List<Hall> listHalls() {
		return persistence.listHalls();
	}

	@Override
	public void removeHall(Integer id) {
		persistence.removeHall(id);
		
	}

	@Override
	public void addConcentrator(Concentrator concentrator) {
		persistence.addConcentrator(concentrator);
		
	}

	@Override
	public List<Concentrator> listConcentrators() {
		return persistence.listConcentrators();
	}

	@Override
	public void removeConcentrator(Integer id) {
		persistence.removeConcentrator(id);
		
	}

	@Override
	public List<Hall> getHalls(int idCompany) {
		return persistence.getHalls(idCompany);
	}

	@Override
	public List<Concentrator> getConcentrators(int idHall) {
		
		return persistence.getConcentrators(idHall);
	}

	@Override
	public List<MonitorData> getMonitorDatas(int idConcentrator) {
		return persistence.getMonitorDatas(idConcentrator);
	}
	
	@Override
	public List<SensorData> getSensorDatas(int idMonitorData) {
		return persistence.getSensorDatas(idMonitorData);
	}

	@Override
	public List<SensorData> listSensorDatas() {
		return persistence.listSensorDatas();
	}

}
