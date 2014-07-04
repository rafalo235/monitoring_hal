package com.hall.monitor.engine.converter;

import java.util.ArrayList;

import com.hall.monitor.protocol.EConfigurationType;
import com.hall.monitor.protocol.EDangerLevel;
import com.hall.monitor.protocol.EMessageType;
import com.hall.monitor.protocol.EReceiveStatus;
import com.hall.monitor.protocol.ESensorState;
import com.hall.monitor.protocol.EValueType;
import com.hall.monitor.protocol.CConfiguration;
import com.hall.monitor.protocol.CConfigurationResponse;
import com.hall.monitor.protocol.CConfigurationValue;
import com.hall.monitor.protocol.CData;
import com.hall.monitor.protocol.CMonitorData;
import com.hall.monitor.protocol.CProtocol;
import com.hall.monitor.protocol.CSensorData;
import com.hall.monitor.protocol.CServerRequest;
import com.hall.monitor.protocol.CServerResponse;
import com.hall.monitor.protocol.IMessage;

/**
 * Konwertuje tablice bajtow na obiekt protokolu i odwrotnie
 * @author Marcin Serwach
 *
 */
public class ProtocolConverter
{
  
  /**
   * Konwertuje bajty na SData
   * @param stream stream 
   * @param idConcentrator id koncentratora
   * @param idSensor id czujnka
   * @param idData id pomiaru
   * @return obiekt SData
   * @throws ParserException niepoprawna struktura bajtow - typ nie odnaleziony
   */
  private static CData converToSData(DataByteInputStream stream,
      int idConcentrator, int idSensor, long idData) throws ParserException {
    
    byte valueTypeBuf = stream.readInt8();
    EValueType valueType = EValueType.convert(valueTypeBuf);
    if (valueType == null) {
      throw new ParserException("Unknown value type", idConcentrator, idSensor,
          idData);
    }
    Object value = null;
    switch (valueType)
    {
    case INT_8:
      value = stream.readInt8();
      break;
    case UINT_8:
      value = stream.readUInt8();
      break;
    case INT_16:
      value = stream.readInt16();
      break;
    case UINT_16:
      value = stream.readUInt16();
      break;
    case INT_32:
      value = stream.readInt32();
      break;
    case UINT_32:
      value = stream.readUInt32();
      break;
    case INT_64:
    case UINT_64:
      value = stream.readInt64();
      break;
    case FLOAT_32:
      value = stream.readFloat32();
      break;
    case DOUBLE_64:
      value = stream.readDouble64();
      break;
    case VOID:
      value = CData.VOID_VALUE;
      break;
    }
    
    return new CData(valueType, value);
    
  }
  
  /**
   * Konwertuje bajty na SSensorData
   * @param stream stream 
   * @param idConcentrator id koncentratora
   * @return objekt SSensorData
   * @throws ParserException gdy nieznany jest stan lub poziom niebezpieczenstwa (niepoprawne wartosci)
   */
  private static CSensorData convertToSSensorData(DataByteInputStream stream,
      int idConcentrator) throws ParserException {
    
    long idData = stream.readUInt32();
    char idSensor = stream.readUInt8();
    long timeStamp = stream.readInt64();
    byte sensorStateBuf = stream.readInt8();
    ESensorState sensorState = ESensorState.convert(sensorStateBuf);
    
    if (sensorState == null) {
      throw new ParserException("Unknown sensor state", idConcentrator,
          idSensor, idData);
    }
    
    byte dangerLevelBuf = stream.readInt8();
    EDangerLevel dangerLevel = EDangerLevel.convert(dangerLevelBuf);
    if (dangerLevel == null) {
      throw new ParserException("Unknown danger level", idConcentrator,
          idSensor, idData);
    }
    
    CData data = converToSData(stream, idConcentrator, idSensor, idData);
    
    return new CSensorData(idData, idSensor, timeStamp, sensorState,
        dangerLevel, data);
  }
  
  /**
   * Konwertuje bajty do SMonitorData
   * @param stream stream
   * @param idConcentrator id koncentratatora
   * @return object SMonitorData
   * @throws ParserException gdy stan danych czujnikow jest niepoprawny
   */
  private static CMonitorData convertToSMonitorData(DataByteInputStream stream,
      int idConcentrator) throws ParserException {
    
    long sendTime = stream.readInt64();
    char sensorsAmount = stream.readUInt8();
    long sensorsDataSize = stream.readUInt32();
    ArrayList<CSensorData> sensorDates = new ArrayList<CSensorData>(
        (int) sensorsDataSize);
    for (int i = 0; i < sensorsDataSize; ++i) {
      
      CSensorData sensorData = convertToSSensorData(stream, idConcentrator);
      sensorDates.add(sensorData);
    }
    
    return new CMonitorData(sendTime, sensorsAmount, sensorDates);
  }
  
  /**
   * Konwertuje bajty na pojedyncza konfiguracje (pojedyncza opcja konfiguracyjna)
   * @param stream stream
   * @param idConcentrator id koncentratora
   * @return object SConfigurationValue
   * @throws ParserException niepoprawny typ danych
   */
  private static CConfigurationValue convertToSConfigurationValue(
      DataByteInputStream stream, int idConcentrator) throws ParserException {
    
    char idSensor = stream.readUInt8();
    byte configurationTypeBuf = stream.readInt8();
    EConfigurationType configurationType = EConfigurationType
        .convert(configurationTypeBuf);
    CData data = converToSData(stream, idConcentrator,
        ParserException.ID_UKNOWN, ParserException.ID_UKNOWN);
    return new CConfigurationValue(idSensor, configurationType, data);
  }
  
  /**
   * Konwertuje bajty na objekt skupiajacy opcje konfiguracyjne SConfiguration
   * @param stream stream 
   * @param idConcentrator id koncentratora 
   * @return object SConfiguration
   * @throws ParserException gdy niepoprawne sa typy danych opcji
   */
  private static CConfiguration convertToSConfiguration(
      DataByteInputStream stream, int idConcentrator) throws ParserException {
    
    char configurationSize = stream.readUInt8();
    ArrayList<CConfigurationValue> configurations = new ArrayList<CConfigurationValue>();
    for (int i = 0; i < configurationSize; ++i) {
      CConfigurationValue buf = convertToSConfigurationValue(stream,
          idConcentrator);
      configurations.add(buf);
    }
    
    return new CConfiguration(configurations);
  }
  
  /**
   * Konwertuje bajty na obiekt opowiedzi konfiguracji SConfigurationResponse
   * @param stream stream
   * @param idConcentrator id koncentratora
   * @return obiekt SConfgiruationResponse
   * @throws ParserException blad struktury
   */
  private static CConfigurationResponse convertoSConfigurationResponse(
      DataByteInputStream stream, int idConcentrator) throws ParserException {
    
    byte receiveStatusBuf = stream.readInt8();
    
    EReceiveStatus receiveStatus = EReceiveStatus.convert(receiveStatusBuf);
    if (receiveStatus == null) {
      throw new ParserException("Unknown receive status", idConcentrator,
          ParserException.ID_UKNOWN, ParserException.ID_UKNOWN);
    }
    
    long idRequestPackage = stream.readUInt32();
    CConfiguration currentConfiguration = convertToSConfiguration(stream,
        idConcentrator);
    return new CConfigurationResponse(receiveStatus, idRequestPackage,
        currentConfiguration);
  }
  
  /**
   * @deprecated typ pakietu wysylany z koncentratora do serwera
   * Konwersja bajtow na zadnei koncentratora przeslania konfiguracji SServerRequest
   * @param stream stream 
   * @param idConcentrator id koncentratora
   * @return object SServerRequest
   * @throws ParserException niepoprawny typ konfiguracji
   */
  private static CServerRequest convertToSServerRequest(
      DataByteInputStream stream, int idConcentrator) throws ParserException {
    
    return null;
  }
  
  private static CServerResponse convertTOSServerResponse(
      DataByteInputStream stream, int idConcentrator) throws ParserException {
    
    return null;
  }
  
  /**
   * Konwertuje tablice bajtow na obiekty 
   * @param bytes tablica bajtow
   * @return obiekt protokolu
   * @throws ParserException bledy struktury lub blad CRC
   */
  public static CProtocol convertToProtocol(byte bytes[])
      throws ParserException {
    
    DataByteInputStream stream = new DataByteInputStream(bytes);
    
    char version = stream.readUInt8();
    if (version != CProtocol.VERSION){
      throw new ParserException("Unsupported version of protocol", 0, 0, 0);
    }
    if (!stream.isValidCRC())
    {
      throw new ParserException("Wrong CRC", 0, 0, 0);
    }
    long size = stream.readUInt32();
    char idConcentrator = stream.readUInt16();
    
    long idPackage = stream.readUInt32();
    byte messageTypeBuf = stream.readInt8();
    EMessageType messageType = EMessageType.convert(messageTypeBuf);
    
    if (messageType == null) {
      throw new ParserException("Unknown message type", idConcentrator,
          ParserException.ID_UKNOWN, ParserException.ID_UKNOWN);
    }
    IMessage message = null;
    switch (messageType)
    {
    case CONFIGURATION_RESPONSE:
      message = convertoSConfigurationResponse(stream, idConcentrator);
      break;
    case MONITOR_DATA:
      message = convertToSMonitorData(stream, idConcentrator);
      break;
    case SERVER_MONITOR_RESPONSE:
      message = convertTOSServerResponse(stream, idConcentrator);
      break;
    case SERVER_REQUEST:
      message = convertToSServerRequest(stream, idConcentrator);
      break;
    }
    char crc = stream.readUInt16();
    
    return new CProtocol(size, idConcentrator, crc, idPackage,
        messageType, message);
  }
  
  // //////////////////////////////////////////////////////////////////
  /**
   * @deprecated SMonitorData wysylane z koncentratora - konwersja do bajtow nie jest potrzebna
   * @param data stream na bajty
   * @param monitor obiekt z pomiarami
   */
  private static void convertToBytes(DataByteOutputStream data,
      CMonitorData monitor) {
    
  }
  /**
   * @deprecated SServerRequest wysylane z koncentratora - konwersja do bajtow nie jest potrzebna
   * @param data stream na bajty
   * @param request zadanie do serwera
   */
  private static void convertToBytes(DataByteOutputStream data,
      CServerRequest request) {
    
  }
  
  /**
   * Konwerrtuje odpowiedz serwera na bajty
   * @param data stream na bajty
   * @param response odpowiedz
   */
  private static void convertToBytes(DataByteOutputStream data,
      CServerResponse response) {
    
    data.writeInt8(response.getStatus().getCppValue());
    data.writeUInt32(response.getIdRequestPackage());
    convertToBytes(data, response.getConfiguration());
  }
  
  /**
   * Konwertuje konfiguracje na bajty
   * @param data stream na bajty
   * @param confResponse konfiguracja
   */
  private static void convertToBytes(DataByteOutputStream data,
      CConfigurationResponse confResponse) {
    
    data.writeInt8(confResponse.getStatus().getCppValue());
    data.writeUInt32(confResponse.getIdRequestPackage());
    convertToBytes(data, confResponse.getCurrentConfiguration());
  }
  
  /**
   * KOnwertuje zbior konfiguracji na tablice bajtow
   * @param data stream - cel
   * @param configuration konfiguracja do skonwerterowania
   */
  private static void convertToBytes(DataByteOutputStream data,
      CConfiguration configuration) {
    
    data.writeUInt8(configuration.getConfigurationSize());
    ArrayList<CConfigurationValue> values = configuration.getConfigurations();
    for (CConfigurationValue value : values){
      convertToBytes(data, value);
    }
  }
  
  /**
   * KOnwertuje pojedyncza opcje konfiguracji na tablice bajtow
   * @param data stream - cel
   * @param value objekt do skonwerterowania
   */
  private static void convertToBytes(DataByteOutputStream data,
      CConfigurationValue value) {
    
    data.writeUInt8(value.getIdSensor());
    data.writeInt8(value.getConfigurationType().getCppValue());
    convertToBytes(data, value.getData());
  }
  
  /**
   * Konwertuje obiekt SData do tablicy bajtow, ktora odpisuje do obiektu DataByteOutputStream
   * @param data stream - cel
   * @param sdata obiekt do skonwerterowania
   */
  private static void convertToBytes(DataByteOutputStream data,
      CData sdata) {
    
    data.writeInt8(sdata.getType().getCppValue());
    Object value = sdata.getValue();
    switch (sdata.getType())
    {
    case INT_8:
      data.writeInt8((Byte) value);
      break;
    case UINT_8:
      data.writeUInt8((Character) value);
      break;
    case INT_16:
      data.writeInt16((Short) value);
      break;
    case UINT_16:
      data.writeUInt16((Character) value);
      break;
    case INT_32:
      data.writeInt32((Integer) value);
      break;
    case UINT_32:
      data.writeUInt32((Long) value);
      break;
    case INT_64:
      data.writeInt64((Long) value);
      break;
    case UINT_64:
      data.writeUInt64((Long) value);
      break;
    case FLOAT_32:
      data.writeFloat32((Float) value);
      break;
    case DOUBLE_64:
      data.writeDouble64((Double) value);
      break;
    case VOID:
      data.writeVoid();
      break;
    }
    
  }
  
  /**
   * Konwertuje protokol do postaci tablicy bajtow
   * @param protocol obiekt protokolu
   * @return tablica bajtow
   */
  public static byte[] convertToBytes(CProtocol protocol) {
    DataByteOutputStream data = new DataByteOutputStream(
        (int) protocol.getSize());
    
    data.writeUInt8(protocol.getVersion());
    data.writeUInt32(protocol.getSize());
    data.writeUInt16(protocol.getIdConcentrator());
    data.writeUInt32(protocol.getIdPackage());
    data.writeInt8(protocol.getType().getCppValue());
    
    switch (protocol.getType())
    {
    case CONFIGURATION_RESPONSE:
      convertToBytes(data, (CConfigurationResponse) protocol.getMessage());
      break;
    case MONITOR_DATA:
      convertToBytes(data, (CMonitorData) protocol.getMessage());
      break;
    case SERVER_MONITOR_RESPONSE:
      convertToBytes(data, (CServerResponse) protocol.getMessage());
      break;
    case SERVER_REQUEST:
      convertToBytes(data, (CServerRequest) protocol.getMessage());
      break;
    }
    
    // oblicz CRC
    data.writeUInt16(protocol.getCrc());
    byte[] crc = data.calcCRC16();
    data.setPosition(data.getPosiotion() - Character.SIZE / 8);
    data.writeByteArray(crc);
    return data.getBytes();
  }
  
  /**
   * Wyjatek dla informowania o bledach struktury pakietu - bledy konwersji z tablicy bajtow na obiek pakietu
   * @author Marcin Serwach
   *
   */
  public static class ParserException extends Exception
  {
    /**
     * 
     */
    private static final long serialVersionUID = 1L;
    
    public static final int   ID_UKNOWN        = -1;
    private int               idConcentrator;
    private int               idSensor;
    private long              idData;
    
    public ParserException(String message, int idConcentrator, int idSensor,
        long idData2)
    {
      super(message);
      
      this.idConcentrator = idConcentrator;
      this.idSensor = idSensor;
      this.idData = idData2;
    }
    
    public static long getSerialversionuid() {
      return serialVersionUID;
    }
    
    public int getIdConcentrator() {
      return idConcentrator;
    }
    
    public int getIdSensor() {
      return idSensor;
    }
    
    public long getIdData() {
      return idData;
    }
    
  }
  
  public static class CRCException extends Exception
  {
    
    /**
     * 
     */
    private static final long serialVersionUID = 1L;
    
  }
}
