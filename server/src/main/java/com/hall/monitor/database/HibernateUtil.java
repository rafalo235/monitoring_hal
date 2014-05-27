package com.hall.monitor.database;

import java.util.logging.Level;
import java.util.logging.Logger;

import org.hibernate.SessionFactory;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;

/**
 * http://viralpatel.net/blogs/hibernate-one-to-many-annotation-tutorial/
 * 
 * @author iblis
 * 
 */
public class HibernateUtil
{
  
  private static SessionFactory factory = buildSessionFactory();
  private static final Logger   log     = Logger.getLogger(HibernateUtil.class
                                            .getSimpleName());
  
  public static SessionFactory getFactory() {
    return factory;
  }
  
  private static SessionFactory buildSessionFactory() {
    try {
      // Create the SessionFactory from hibernate.cfg.xml
      Configuration configuration = new Configuration().configure();
      StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder()
          .applySettings(configuration.getProperties());
      SessionFactory factory = configuration.buildSessionFactory(builder
          .build());
      
      return factory;
    }
    catch (Throwable ex) {
      log.log(Level.SEVERE, "Initial SessionFactory creation failed." + ex);
      throw new ExceptionInInitializerError(ex);
    }
  }
  
}
