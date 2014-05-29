package com.hall.monitor.engine.converter;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Java nie ma wbudowanego stream'a z narzuconym Little Endian.
 * 
 * @author iblis
 * 
 */
public class DataByteInputStream
{
  
  private ByteBuffer buffer;
  
  public DataByteInputStream(byte bytes[])
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
  
  public short readInt16() {
    return buffer.getShort();
  }
  
  public char readUInt16() {
    return buffer.getChar();
  }
  
  public int readInt32() {
    return buffer.getInt();
  }
  
  public long readUInt32() {
    return (char) (buffer.getInt() & 0xFFFFFFFFL);
  }
  
  public long readInt64() {
    return buffer.getLong();
  }
  
  public float readFloat32() {
    return buffer.getFloat();
  }
  
  public double readDouble64() {
    return buffer.getDouble();
  }
  
  public void setPosition(int newPosition) {
    buffer.position(newPosition);
  }
  
  public int getPosiotion() {
    return buffer.position();
  }
  
}
