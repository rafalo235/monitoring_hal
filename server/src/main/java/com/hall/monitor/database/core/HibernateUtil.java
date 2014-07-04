package com.hall.monitor.database.core;

import java.util.logging.Level;
import java.util.logging.Logger;

import org.hibernate.SessionFactory;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;

/**
 * klasa wspomagaja hibernatea
 * @author Marcin Serwach
 *
 */
public class HibernateUtil
{
  /** fabryka sesji */
  private static SessionFactory factory = buildSessionFactory();
  
  /** logger */
  private static final Logger   log     = Logger.getLogger(HibernateUtil.class
                                            .getSimpleName());
  
  /**
   * Zwraca fabryke sesji hibernatea
   * @return fabryka sesji
   */
  public static SessionFactory getFactory() {
    return factory;
  }
  
  /**
   * Zwraca fabryke sesji. Hibernate tworzy to co potrzebuje (tabele)
   * @return fabryka sesji
   */
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
