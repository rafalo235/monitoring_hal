package com.hall.monitor.engine.converter;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class DataByteOutputStream
{
  private ByteBuffer buffer;
  public static final byte VoidValue = (byte) 0xFF;
  public DataByteOutputStream(int size)
  {
    buffer = ByteBuffer.allocate(size);
    buffer.order(ByteOrder.LITTLE_ENDIAN);
  }
  
  public void writeInt8(byte value) {
    buffer.put(value);
  }
  
  public void writeUInt8(char value) {
    byte buf = (byte)(value & 0xFF);
    buffer.put(buf);
  }
  
  public void writeInt16(short value) {
    buffer.putShort(value);
  }
  
  public void writeUInt16(char value) {
    buffer.putChar(value);
  }
  
  public void writeInt32(int value) {
    buffer.putInt(value);
  }
  
  public void writeUInt32(long value) {
    int buf = (int)(value & 0xFFFFFFFF);
    buffer.putInt(buf);
  }
  
  public void writeInt64(long value) {
    buffer.putLong(value);
  }
  
  public void writeUInt64(long value) {
    buffer.putLong(value);
  }
  
  public void writeFloat32(float value) {
    buffer.putFloat(value);
  }
  
  public void writeDouble64(double value) {
    buffer.putDouble(value);
  }
  
  public void writeVoid(){
    buffer.put(VoidValue);
  }
  public void setPosition(int newPosition) {
    buffer.position(newPosition);
  }
  
  public int getPosiotion() {
    return buffer.position();
  }
  
  public byte[] getBytes(){
    return buffer.array();
  }
  
  public char countCrc16() {
	  byte[] copy = buffer.array().clone();
	  byte[] divisor = { (byte) 0xFE, (byte) 0xEF, (byte) 0x80 }; //TODO
	  char crc = 0x0000;
	  
	  DataByteInputStream.xorBytes(copy, copy.length - 2, divisor);
	  crc = (char) ((copy[1] << 8) + copy[0]);
	  
	  return crc;
  }
}
