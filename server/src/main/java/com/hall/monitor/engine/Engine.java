package com.hall.monitor.engine;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.hall.monitor.database.DBManager;
import com.hall.monitor.database.data.Concentrator;
import com.hall.monitor.engine.converter.ProtocolConverter;
import com.hall.monitor.engine.converter.ProtocolConverter.ParserException;
import com.hall.monitor.engine.sms.SMSManager;
import com.hall.monitor.protocol.EDangerLevel;
import com.hall.monitor.protocol.EMessageType;
import com.hall.monitor.protocol.EReceiveStatus;
import com.hall.monitor.protocol.SConfiguration;
import com.hall.monitor.protocol.SConfigurationResponse;
import com.hall.monitor.protocol.SMonitorData;
import com.hall.monitor.protocol.SProtocol;
import com.hall.monitor.protocol.SRequest;
import com.hall.monitor.protocol.SSensorData;
import com.hall.monitor.protocol.SServerRequest;
import com.hall.monitor.protocol.SServerResponse;
import com.hall.monitor.protocol.UMessage;

public class Engine
{
  private DBManager db  = new DBManager();
  private Logger    log = Logger.getLogger(Engine.class.getSimpleName());
  private SMSManager smsManager = new SMSManager();
  
  // TODO: jak zwiekszac id pakietow
  
  private long      idPackage;

  
  /**
   * Glowna funkcja serwera do obslugi danych z koncentratora. 
   * Wywoluje funkcje konwertujace tablice bajtow na protokol.
   * Nastepie wowoluje funkcje specyficzne dla typow protokolu.
   * @param bytes tablica bajtow z koncentratora
   * @return
   */
  public byte[] onReceiveData(byte bytes[]) {
    // najpierw konwersja do obiektu protokolu
    SProtocol responseProtocol = null;
    try {
      SProtocol protocol = ProtocolConverter.convertToProtocol(bytes);
      log.log(Level.SEVERE,
          "Protocol data received: id=" + protocol.getIdPackage() + " type: "+protocol.getType());
      UMessage message = protocol.getMessage();
      if (message instanceof SMonitorData){
        responseProtocol = operateMonitorData(protocol);
      }
      else if(message instanceof SServerRequest){
        responseProtocol =  operateSeverRequest(protocol);
      }
      else if(message instanceof SConfigurationResponse){
      responseProtocol =  operateConfigurationResponse(protocol);
      }
      
    }
    catch (ParserException e) {
      log.log(Level.SEVERE, "Protocol parser error ");
      e.printStackTrace();
      responseProtocol =  getWrongProtocolResponse(EReceiveStatus.BAD_STRUCTURE, SProtocol.ERROR_ID_CONCENTRATOR);
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
  
  private SProtocol operateConfigurationResponse(SProtocol protocol){
    if (!db.store(protocol)){
      return getWrongProtocolResponse(EReceiveStatus.OPERATION_FAILED, protocol.getIdConcentrator());
    }
    return null;
  }
  /**
   * Funkcja obslugujaca zadanie przeslania konfiguacji do koncentratora.
   * @param protocol protokol z konfiguracja
   * @return
   */
  private SProtocol operateSeverRequest(SProtocol protocol){
    if (!db.store(protocol)){
      return getWrongProtocolResponse(EReceiveStatus.OPERATION_FAILED, protocol.getIdConcentrator());
    }
    List<SRequest> requests = ((SServerRequest)protocol.getMessage()).getRequests();
    SConfiguration conf = db.loadConcentratorConfiguration(protocol.getIdConcentrator(), requests);
    SServerResponse response = new SServerResponse(EReceiveStatus.OK, protocol.getIdPackage(), conf);
    return new SProtocol(0, protocol.getIdConcentrator(), (char)0, idPackage, response);
  }
  
  /**
   * Funkcja obslugujaca dane z czujnikow od koncentratora
   * @param protocol
   * @return
   */
  private SProtocol operateMonitorData(SProtocol protocol){
    wasDangerLvl(protocol);
    if (!db.store(protocol)){
      return getWrongProtocolResponse(EReceiveStatus.OPERATION_FAILED, protocol.getIdConcentrator());
    }
    
    SConfiguration conf = db.loadConcentratorConfiguration(protocol.getIdConcentrator());
    SServerResponse response = new SServerResponse(EReceiveStatus.OK, protocol.getIdPackage(), conf);
    return new SProtocol(0, protocol.getIdConcentrator(), (char)0, idPackage, response);
  }
  
  /**
   * Tworzy protokol odpowiedzi jesli protokol otrzymany byl niepoprawny
   * @return
   */
  private SProtocol getWrongProtocolResponse(EReceiveStatus status, char idConcentrator) {

    SServerResponse response = new SServerResponse(
        status, (char) 0);
    return new SProtocol(0, idConcentrator, (char) 0, idPackage++,
        EMessageType.SERVER_MONITOR_RESPONSE, response);
    
  }
  
  /**
   * Sprawdza, czy dane sa niebezpiecznie.
   * Jesli sa niebezpieczne wartosci, to wywoluje funkcje wysylajaca sms.
   * @param protocol
   * @return true, jesli byl niebezpieczny lvl
   */
  private boolean wasDangerLvl(SProtocol protocol){
    SMonitorData monitor = (SMonitorData) protocol.getMessage();
    ArrayList<SSensorData> datas = monitor.getSensorsData();
    HashSet<Integer> warningSensorsIds = new HashSet<Integer>();
    HashSet<Integer> alarmSensorsIds = new HashSet<Integer>();
    
    for(SSensorData sensorData : datas){
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
