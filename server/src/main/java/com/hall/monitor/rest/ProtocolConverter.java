package com.hall.monitor.rest;

import java.util.ArrayList;

import com.hall.monitor.protocol.EConfigurationType;
import com.hall.monitor.protocol.EDangerLevel;
import com.hall.monitor.protocol.EMessageType;
import com.hall.monitor.protocol.EReceiveStatus;
import com.hall.monitor.protocol.ESensorState;
import com.hall.monitor.protocol.EValueType;
import com.hall.monitor.protocol.SConfiguration;
import com.hall.monitor.protocol.SConfigurationResponse;
import com.hall.monitor.protocol.SConfigurationValue;
import com.hall.monitor.protocol.SData;
import com.hall.monitor.protocol.SMonitorData;
import com.hall.monitor.protocol.SProtocol;
import com.hall.monitor.protocol.SSensorData;
import com.hall.monitor.protocol.SServerRequest;
import com.hall.monitor.protocol.SServerResponse;
import com.hall.monitor.protocol.UMessage;

public class ProtocolConverter
{
  
  private static SData converToSData(DataByteStream stream, int idConcentrator,
      int idSensor, long idData) throws ParserException {
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
      value = SData.VOID_VALUE;
      break;
    }
    
    return new SData(valueType, value);
    
  }
  
  private static SSensorData convertToSSensorData(DataByteStream stream,
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
    
    SData data = converToSData(stream, idConcentrator, idSensor, idData);
    
    return new SSensorData(idData, idSensor, timeStamp, sensorState,
        dangerLevel, data);
  }
  
  private static SMonitorData convertToSMonitorData(DataByteStream stream,
      int idConcentrator) throws ParserException {
    long sendTime = stream.readInt64();
    char sensorsAmount = stream.readUInt8();
    long sensorsDataSize = stream.readUInt32();
    ArrayList<SSensorData> sensorDates = new ArrayList<SSensorData>(
        (int) sensorsDataSize);
    for (int i = 0; i < sensorsDataSize; ++i) {
      
      SSensorData sensorData = convertToSSensorData(stream, idConcentrator);
      sensorDates.add(sensorData);
    }
    
    return new SMonitorData(sendTime, sensorsAmount, sensorDates);
  }
  
  private static SConfigurationValue convertToSConfigurationValue(
      DataByteStream stream, int idConcentrator) throws ParserException {
    byte idSensor = stream.readInt8();
    byte configurationTypeBuf = stream.readInt8();
    EConfigurationType configurationType = EConfigurationType
        .convert(configurationTypeBuf);
    SData data = converToSData(stream, idConcentrator,
        ParserException.ID_UKNOWN, ParserException.ID_UKNOWN);
    return new SConfigurationValue(idSensor, configurationType, data);
  }
  
  private static SConfiguration convertToSConfiguration(DataByteStream stream,
      int idConcentrator) throws ParserException {
    char configurationSize = stream.readUInt8();
    ArrayList<SConfigurationValue> configurations = new ArrayList<SConfigurationValue>();
    for (int i = 0; i < configurationSize; ++i) {
      SConfigurationValue buf = convertToSConfigurationValue(stream,
          idConcentrator);
      configurations.add(buf);
    }
    
    return new SConfiguration(configurations);
  }
  
  private static SConfigurationResponse convertoSConfigurationResponse(
      DataByteStream stream, int idConcentrator) throws ParserException {
    
    byte receiveStatusBuf = stream.readInt8();
    
    EReceiveStatus receiveStatus = EReceiveStatus.convert(receiveStatusBuf);
    if (receiveStatus == null) {
      throw new ParserException("Unknown receive status", idConcentrator,
          ParserException.ID_UKNOWN, ParserException.ID_UKNOWN);
    }
    
    long idRequestPackage = stream.readUInt32();
    SConfiguration currentConfiguration = convertToSConfiguration(stream,
        idConcentrator);
    return new SConfigurationResponse(receiveStatus, idRequestPackage,
        currentConfiguration);
  }
  
  private static SServerRequest convertToSServerRequest(DataByteStream stream,
      int idConcentrator) throws ParserException {
    
    return null;
  }
  
  private static SServerResponse convertTOSServerResponse(
      DataByteStream stream, int idConcentrator) throws ParserException {
    
    return null;
  }
  
  public static SProtocol convertToProtocol(byte bytes[])
      throws ParserException {
    DataByteStream stream = new DataByteStream(bytes);
    
    char version = stream.readUInt8();
    long size = stream.readUInt32();
    int idConcentrator = stream.readUInt16();
    
    long idPackage = stream.readUInt32();
    byte messageTypeBuf = stream.readInt8();
    EMessageType messageType = EMessageType.convert(messageTypeBuf);
    
    if (messageType == null) {
      throw new ParserException("Unknown message type", idConcentrator,
          ParserException.ID_UKNOWN, ParserException.ID_UKNOWN);
    }
    UMessage message = null;
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
    int crc = stream.readUInt16();
    return new SProtocol(version, size, idConcentrator, crc, idPackage,
        messageType, message);
  }
  
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
