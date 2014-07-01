package com.monitoring.hall.persistence;

import java.util.List;

import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import com.monitoring.hall.beans.Company;
import com.monitoring.hall.beans.Concentrator;
import com.monitoring.hall.beans.Hall;
import com.monitoring.hall.beans.MonitorData;
import com.monitoring.hall.beans.SensorData;

@Repository
@SuppressWarnings("unchecked")
public class PersistenceDAOImpl implements PersistenceDAO {

	@Autowired
	private SessionFactory sessionFactory;
	 
	 
	@Override
	public void addCompany(Company companies) {
		sessionFactory.getCurrentSession().save(companies);
	}

	@Override
	public List<Company> listCompany() {
		return sessionFactory.getCurrentSession().createQuery("from Company").list();
	}

	@Override
	public void removeCompany(Integer id) {
		 Company company = (Company) sessionFactory.getCurrentSession().load(
	                Company.class, id);
	        if (null != company) {
	            sessionFactory.getCurrentSession().delete(company);
	        }
	}

	@Override
	public void addHall(Hall hall) {
		sessionFactory.getCurrentSession().save(hall);
		
	}

	@Override
	public List<Hall> listHalls() {
		return sessionFactory.getCurrentSession().createQuery("from Hall").list();
	}

	@Override
	public void removeHall(Integer id) {
		 Hall hall = (Hall) sessionFactory.getCurrentSession().load(
	                Hall.class, id);
	        if (null != hall) {
	            sessionFactory.getCurrentSession().delete(hall);
	        }
	}

	@Override
	public void addConcentrator(Concentrator concentrator) {
		sessionFactory.getCurrentSession().save(concentrator);
		
	}

	@Override
	public List<Concentrator> listConcentrators() {
		return sessionFactory.getCurrentSession().createQuery("from Concentrator").list();
	}

	@Override
	public void removeConcentrator(Integer id) {
		 Concentrator concentrator = (Concentrator) sessionFactory.getCurrentSession().load(
				 Concentrator.class, id);
	        if (null != concentrator) {
	            sessionFactory.getCurrentSession().delete(concentrator);
	        }
		
	}
	
	
	@Override
	  public List<Hall> getHalls(int idCompany) {
	     return sessionFactory.getCurrentSession().createQuery("from Hall where idCompany="+idCompany).list();
	  }

	@Override
	public List<Concentrator> getConcentrators(int idHall) {
		 return sessionFactory.getCurrentSession().createQuery("from Concentrator where idHall="+idHall).list();
	}

	@Override
	public List<MonitorData> getMonitorDatas(int idConcentrator) {
		return sessionFactory.getCurrentSession().createQuery("from MonitorData where idConcentrator="+idConcentrator).list();
	}

	@Override
	public List<SensorData> getSensorDatas(int idMonitorData) {
		return sessionFactory.getCurrentSession().createQuery("from SensorData where idMonitorData="+idMonitorData).list();
	}

	@Override
	public List<SensorData> listSensorDatas() {
		return sessionFactory.getCurrentSession().createQuery("from SensorData").list();
	}

}
