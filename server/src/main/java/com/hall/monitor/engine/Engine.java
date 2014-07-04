package com.hall.monitor.engine;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.hall.monitor.database.core.DBManager;
import com.hall.monitor.database.core.data.Concentrator;
import com.hall.monitor.engine.converter.ProtocolConverter;
import com.hall.monitor.engine.converter.ProtocolConverter.ParserException;
import com.hall.monitor.engine.sms.SMSManager;
import com.hall.monitor.protocol.EDangerLevel;
import com.hall.monitor.protocol.EMessageType;
import com.hall.monitor.protocol.EReceiveStatus;
import com.hall.monitor.protocol.CConfiguration;
import com.hall.monitor.protocol.CConfigurationResponse;
import com.hall.monitor.protocol.CMonitorData;
import com.hall.monitor.protocol.CProtocol;
import com.hall.monitor.protocol.CRequest;
import com.hall.monitor.protocol.CSensorData;
import com.hall.monitor.protocol.CServerRequest;
import com.hall.monitor.protocol.CServerResponse;
import com.hall.monitor.protocol.IMessage;

/**
 * Klasa silnika rozpoznawania i reagowania na pakiety przeslane z koncentratora oraz tworzaca odpowiedzi
 * @author Marcin Serwach
 *
 */
public class Engine
{
  /** manadzer bazy */
  private DBManager db  = new DBManager();
  
  /** logger */
  private Logger    log = Logger.getLogger(Engine.class.getSimpleName());
  
  /** manadzer smsow */
  private SMSManager smsManager = new SMSManager();
  
  /** id paczki */
  private long      idPackage;

  
  /**
   * Glowna funkcja serwera do obslugi danych z koncentratora. 
   * Wywoluje funkcje konwertujace tablice bajtow na protokol.
   * Nastepie wowoluje funkcje specyficzne dla typow protokolu.
   * @param bytes tablica bajtow z koncentratora
   * @return tablica bajtow zawierajaca odpowiedz serwera
   */
  public byte[] onReceiveData(byte bytes[]) {
    // najpierw konwersja do obiektu protokolu
    CProtocol responseProtocol = null;
    try {
      CProtocol protocol = ProtocolConverter.convertToProtocol(bytes);
      log.log(Level.SEVERE,
          "Protocol data received: id=" + protocol.getIdPackage() + " type: "+protocol.getType());
      IMessage message = protocol.getMessage();
      if (message instanceof CMonitorData){
        responseProtocol = operateMonitorData(protocol);
      }
      else if(message instanceof CServerRequest){
        responseProtocol =  operateSeverRequest(protocol);
      }
      else if(message instanceof CConfigurationResponse){
      responseProtocol =  operateConfigurationResponse(protocol);
      }
      
    }
    catch (ParserException e) {
      log.log(Level.SEVERE, "Protocol parser error ");
      e.printStackTrace();
      responseProtocol =  getWrongProtocolResponse(EReceiveStatus.BAD_STRUCTURE, CProtocol.ERROR_ID_CONCENTRATOR);
    }
    
    if (responseProtocol == null){
      // odpowiedz na potwierdzenie konfiguracji
      return null;
    }
    else
    {
      return ProtocolConverter.convertToBytes(responseProtocol);
    }
  }
  
  /**
   * Obsluguje potwierdzenie zmiany konfiguracji
   * @param protocol protol
   * @return potwierdzenie
   */
  private CProtocol operateConfigurationResponse(CProtocol protocol){
    if (!db.store(protocol)){
      return getWrongProtocolResponse(EReceiveStatus.OPERATION_FAILED, protocol.getIdConcentrator());
    }
    return null;
  }
  /**
   * Funkcja obslugujaca zadanie przeslania konfiguacji do koncentratora.
   * @param protocol protokol z konfiguracja
   * @return protokol z odpowiedzia
   */
  private CProtocol operateSeverRequest(CProtocol protocol){
    if (!db.store(protocol)){
      return getWrongProtocolResponse(EReceiveStatus.OPERATION_FAILED, protocol.getIdConcentrator());
    }
    List<CRequest> requests = ((CServerRequest)protocol.getMessage()).getRequests();
    CConfiguration conf = db.getConcentratorConfiguration(protocol.getIdConcentrator(), requests);
    CServerResponse response = new CServerResponse(EReceiveStatus.OK, protocol.getIdPackage(), conf);
    return new CProtocol(0, protocol.getIdConcentrator(), (char)0, idPackage, response);
  }
  
  /**
   * Funkcja obslugujaca dane z czujnikow od koncentratora
   * @param protocol protokol
   * @return protokol potwierdzenia z ewentualna konfiguracja
   */
  private CProtocol operateMonitorData(CProtocol protocol){
    wasDangerLvl(protocol);
    if (!db.store(protocol)){
      return getWrongProtocolResponse(EReceiveStatus.OPERATION_FAILED, protocol.getIdConcentrator());
    }
    
    CConfiguration conf = db.getConcentratorConfiguration(protocol.getIdConcentrator());
    CServerResponse response = new CServerResponse(EReceiveStatus.OK, protocol.getIdPackage(), conf);
    return new CProtocol(0, protocol.getIdConcentrator(), (char)0, idPackage, response);
  }
  
  /**
   * Tworzy protokol odpowiedzi jesli protokol otrzymany byl niepoprawny
   * @param status status otrzymanego protokolu
   * @param idConcentrator id koncentratora
   * @return protokol z odpowiedzia
   */
  private CProtocol getWrongProtocolResponse(EReceiveStatus status, char idConcentrator) {

    CServerResponse response = new CServerResponse(
        status, (char) 0);
    return new CProtocol(0, idConcentrator, (char) 0, idPackage++,
        EMessageType.SERVER_MONITOR_RESPONSE, response);
    
  }
  
  /**
   * Sprawdza, czy dane sa niebezpiecznie.
   * Jesli sa niebezpieczne wartosci, to wywoluje funkcje wysylajaca sms.
   * @param protocol protokol
   * @return true, jesli byl niebezpieczny lvl
   */
  private boolean wasDangerLvl(CProtocol protocol){
    CMonitorData monitor = (CMonitorData) protocol.getMessage();
    ArrayList<CSensorData> datas = monitor.getSensorsData();
    HashSet<Integer> warningSensorsIds = new HashSet<Integer>();
    HashSet<Integer> alarmSensorsIds = new HashSet<Integer>();
    
    for(CSensorData sensorData : datas){
      if (sensorData.getDangerLevel() == EDangerLevel.ALARM){
        int id = sensorData.getIdSensor();
        alarmSensorsIds.add(id);
      }
      else if (sensorData.getDangerLevel() == EDangerLevel.DANGER)
      {
        int id = sensorData.getIdSensor();
        warningSensorsIds.add(id);
      }
    }
    
    // czy byly jakies niepokojace dane?
    if (warningSensorsIds.isEmpty() && alarmSensorsIds.isEmpty()){
      // nie bylo
      return false;
    }
    else{
      Concentrator concentrator = db.getConcentrator(protocol.getIdConcentrator());
      if (!db.wasLastSeriesDangerous(concentrator.getIdConcentrator()))
      {
        smsManager.sendSms(concentrator, warningSensorsIds, alarmSensorsIds);
      }
      
      return true;
    }
  }
}
