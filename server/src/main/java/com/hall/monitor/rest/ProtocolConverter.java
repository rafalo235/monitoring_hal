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
  
  private static SData converToSData(DataByteInputStream stream,
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
      value = SData.VOID_VALUE;
      break;
    }
    
    return new SData(valueType, value);
    
  }
  
  private static SSensorData convertToSSensorData(DataByteInputStream stream,
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
  
  private static SMonitorData convertToSMonitorData(DataByteInputStream stream,
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
      DataByteInputStream stream, int idConcentrator) throws ParserException {
    
    byte idSensor = stream.readInt8();
    byte configurationTypeBuf = stream.readInt8();
    EConfigurationType configurationType = EConfigurationType
        .convert(configurationTypeBuf);
    SData data = converToSData(stream, idConcentrator,
        ParserException.ID_UKNOWN, ParserException.ID_UKNOWN);
    return new SConfigurationValue(idSensor, configurationType, data);
  }
  
  private static SConfiguration convertToSConfiguration(
      DataByteInputStream stream, int idConcentrator) throws ParserException {
    
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
      DataByteInputStream stream, int idConcentrator) throws ParserException {
    
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
  
  private static SServerRequest convertToSServerRequest(
      DataByteInputStream stream, int idConcentrator) throws ParserException {
    
    return null;
  }
  
  private static SServerResponse convertTOSServerResponse(
      DataByteInputStream stream, int idConcentrator) throws ParserException {
    
    return null;
  }
  
  public static SProtocol convertToProtocol(byte bytes[])
      throws ParserException {
    
    DataByteInputStream stream = new DataByteInputStream(bytes);
    
    char version = stream.readUInt8();
    long size = stream.readUInt32();
    char idConcentrator = stream.readUInt16();
    
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
    char crc = stream.readUInt16();
    return new SProtocol(version, size, idConcentrator, crc, idPackage,
        messageType, message);
  }
  
  // //////////////////////////////////////////////////////////////////
  
  private static void convertToBytes(DataByteOutputStream data,
      SMonitorData monitor) {
    
  }
  
  private static void convertToBytes(DataByteOutputStream data,
      SServerRequest request) {
    
  }
  
  private static void convertToBytes(DataByteOutputStream data,
      SServerResponse response) {
    
    data.writeInt8(response.getStatus().getCppValue());
    data.writeUInt32(response.getIdRequestPackage());
    convertToBytes(data, response.getConfiguration());
  }
  
  private static void convertToBytes(DataByteOutputStream data,
      SConfigurationResponse confResponse) {
    
    data.writeInt8(confResponse.getStatus().getCppValue());
    data.writeUInt32(confResponse.getIdRequestPackage());
    convertToBytes(data, confResponse.getCurrentConfiguration());
  }
  
  private static void convertToBytes(DataByteOutputStream data,
      SConfiguration configuration) {
    
    data.writeUInt8(configuration.getConfigurationSize());
    ArrayList<SConfigurationValue> values = configuration.getConfigurations();
    for (SConfigurationValue value : values){
      convertToBytes(data, value);
    }
  }
  
  private static void convertToBytes(DataByteOutputStream data,
      SConfigurationValue value) {
    
    data.writeInt8(value.getIdSensor());
    data.writeInt8(value.getConfigurationType().getCppValue());
    convertToBytes(data, value.getData());
  }
  
  private static void convertToBytes(DataByteOutputStream data,
      SData sdata) {
    
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
  public static byte[] convertToBytes(SProtocol protocol) {
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
      convertToBytes(data, (SConfigurationResponse) protocol.getMessage());
      break;
    case MONITOR_DATA:
      convertToBytes(data, (SMonitorData) protocol.getMessage());
      break;
    case SERVER_MONITOR_RESPONSE:
      convertToBytes(data, (SServerResponse) protocol.getMessage());
      break;
    case SERVER_REQUEST:
      convertToBytes(data, (SServerRequest) protocol.getMessage());
      break;
    }
    data.writeUInt16(protocol.getCrc());
    return data.getBytes();
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
