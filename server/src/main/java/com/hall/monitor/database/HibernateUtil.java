package com.hall.monitor.database;

import java.util.logging.Level;
import java.util.logging.Logger;

import org.hibernate.SessionFactory;
import org.hibernate.cfg.AnnotationConfiguration;

import com.hall.monitor.database.data.Company;
import com.hall.monitor.database.data.Concentrator;
import com.hall.monitor.database.data.ConcentratorConf;
import com.hall.monitor.database.data.Hall;
import com.hall.monitor.database.data.MonitorData;
import com.hall.monitor.database.data.Request;
import com.hall.monitor.database.data.RequestConf;
import com.hall.monitor.database.data.Sensor;
import com.hall.monitor.database.data.SensorConf;
import com.hall.monitor.database.data.SensorData;
import com.hall.monitor.database.data.User;

/**
 * http://viralpatel.net/blogs/hibernate-one-to-many-annotation-tutorial/
 * 
 * @author iblis
 * 
 */
public class HibernateUtil
{
  
  private static SessionFactory factory = buildSessionFactory();
  private static final Logger   log = Logger.getLogger(HibernateUtil.class
                                        .getSimpleName());
  
  public static SessionFactory getFactory(){
    return factory;
  }
  private static SessionFactory buildSessionFactory() {
    try {
      // Create the SessionFactory from hibernate.cfg.xml
      
      AnnotationConfiguration conf = new AnnotationConfiguration();
      conf.addAnnotatedClass(Company.class);
      conf.addAnnotatedClass(Concentrator.class);
      conf.addAnnotatedClass(ConcentratorConf.class);
      conf.addAnnotatedClass(Hall.class);
      conf.addAnnotatedClass(MonitorData.class);
      conf.addAnnotatedClass(Sensor.class);
      conf.addAnnotatedClass(SensorData.class);
      conf.addAnnotatedClass(Request.class);
      conf.addAnnotatedClass(RequestConf.class);
      conf.addAnnotatedClass(SensorConf.class);
      conf.addAnnotatedClass(User.class);
      
      conf.configure();
      
      return conf.buildSessionFactory();
    }
    catch (Throwable ex) {
      log.log(Level.SEVERE, "Initial SessionFactory creation failed." + ex);
      throw new ExceptionInInitializerError(ex);
    }
  }
  
  
  
}
