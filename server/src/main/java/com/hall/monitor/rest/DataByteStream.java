package com.hall.monitor.rest;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Java nie ma wbudowanego stream'a z narzuconym Little Endian.
 * 
 * @author iblis
 * 
 */
public class DataByteStream
{
  
  private ByteBuffer buffer;
  
  public DataByteStream(byte bytes[])
  {
    buffer = ByteBuffer.wrap(bytes);
    buffer.order(ByteOrder.LITTLE_ENDIAN);
  }
  
  public byte readInt8() {
    return buffer.get();
  }
  
  public char readUInt8() {
    return (char) (buffer.get() & 0xFF);
  }
  
  public char readInt16() {
    return buffer.getChar();
  }
  
  public int readUInt16() {
    return (char) (buffer.getChar() & 0xFFFF);
  }
  
  public int readInt32() {
    return buffer.getInt();
  }
  
  public long readUInt32() {
    return (char) (buffer.getInt() & 0xFFFFFFFFL);
  }
  
  public void setPosition(int newPosition) {
    buffer.position(newPosition);
  }
  
  public int getPosiotion() {
    return buffer.position();
  }
  
}
