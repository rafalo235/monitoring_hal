package com.hall.monitor.database.interfaces;

import java.util.List;

import com.hall.monitor.database.core.data.Company;
import com.hall.monitor.database.core.data.Concentrator;
import com.hall.monitor.database.core.data.ConcentratorConf;
import com.hall.monitor.database.core.data.Hall;
import com.hall.monitor.database.core.data.User;
import com.hall.monitor.protocol.CConfiguration;
import com.hall.monitor.protocol.CProtocol;
import com.hall.monitor.protocol.CRequest;

/**
 * Interfejs menadzera bazy danych
 * @author Marcin Serwach
 *
 */
public interface IDBManager
{
  /**
   * Zwraca konfiguracje koncentratora
   * @param idConcentrator id koncentratora
   * @return konfiguracja
   */
  public CConfiguration getConcentratorConfiguration(int idConcentrator);
  
  /**
   * Zwraca konfiguracje podanych typow dla podanego koncentratora 
   * @param idConcentrator id koncentratora
   * @param requests lista typow konfiguracji do zwrocenenia
   * @return konfiguracja
   */
  public CConfiguration getConcentratorConfiguration(int idConcentrator, List<CRequest> requests);
  
  /**
   * Zapisuje konfiguracje w bazie
   * @param concentratorId id koncentratora
   * @param conf konfiguracja
   * @return true jesli wszystko ok, false jesli nie
   */
  public boolean storeConcentratorConfiguration(int concentratorId, ConcentratorConf conf);
  
  /**
   * dodaje firme do bazy
   * @param name nazwa firmy
   * @param address adres
   * @return obiekt firmy
   */
  public Company addCompany(String name, String address);
  
  /**
   * Zwrac firme
   * @param name nazwa
   * @param address adress
   * @return obiekt firmy
   */
  public Company getCompany(String name, String address);
  
  /**
   * Zwraca liste firm
   * @return lista firm
   */
  public List<Company> getCompanies();
  
  /**
   * Dodaje hale 
   * @param idCompany id firmy
   * @param hallName nazwa hali
   * @param address adres hali
   * @return obiekt hali
   */
  public Hall addHall(int idCompany, String hallName, String address);
  
  /**
   * Zwraca obiekt hali
   * @param idCompany id firmy
   * @param hallName nazwa hali
   * @param address adres hali
   * @return obiekt hali
   */
  public Hall getHall(int idCompany, String hallName, String address);
  
  /**
   * Zwraca liste hall podanej firmy
   * @param idCompany id firmy
   * @return lista hali
   */
  public List<Hall> getHalls(int idCompany);
  
  /**
   * Zwraca liste hali
   * @return lista hali
   */
  public List<Hall> getHalls();
  
  /**
   * Dodaje koncentrator do bazy
   * @param idHall id hali
   * @param idCSensors list z id czujnikow
   * @return okbiekt koncentratora
   */
  public Concentrator addConcentrator(int idHall, List<Integer> idCSensors);
  
  /**
   * Zwraca koncentrator o podanym id
   * @param id id koncentratora
   * @return koncentrator
   */
  public Concentrator getConcentrator(int id);
  
  /**
   * Zapisuje protokol w bazi
   * @param protocol protol
   * @return true jesli wszystko ok; false jesli nie
   */
  public boolean store(CProtocol protocol);
  
  /**
   * Dodaje uzytkownika do bazy 
   * @param firstName imie
   * @param lastName nazwisko
   * @param login login
   * @param pass haslo
   * @return obiekt uzytkownika
   */
  public User addUser(String firstName, String lastName, String login,
      String pass);
  
  /**
   * Zwraca uzytkownika o podanym hasle i loginie
   * @param login login 
   * @param pass haslo
   * @return obiekt uzytkownika
   */
  public User getUser(String login, String pass);

  /**
   * Sprawdza czy ostatnia przeslany pomiar dla podanego koncentratora byl niebezpieczny
   * @param idConcentrator id koncentratora
   * @return true jesli byl niebezpieczny
   */
  public boolean wasLastSeriesDangerous(int idConcentrator);
}
