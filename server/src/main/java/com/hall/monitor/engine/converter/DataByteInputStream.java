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
  
  // Checks CRC16 correctness
  public boolean isValid() {
	  byte[] copy = buffer.array().clone();
	  byte[] divisor = { (byte) 0xFE, (byte) 0xEF, (byte) 0x80 }; //TODO
	  byte tmp;
	  int pointer = copy.length * 8;
	  
	  do {
		  for (int i = 0 ; i < 8 ; i++) {
			  if ( (copy[0] & 0x80) > 0) { // jeżeli najstarszy bit jest 1 oblicz XOR
	                copy[0] ^= divisor[0];
	                copy[1] ^= divisor[1];
	                tmp = copy[2];
	                copy[2] ^= divisor[2];
	                copy[2] &= 0x80;
	                copy[2] |= tmp & 0x7F;
	            }
	            shift(copy); // przesuń bity danych o 1
	            pointer--;
		  }
	  } while (pointer > 0);
	  
	  for (byte b : copy) {
		  if (b != 0) {
			  return false;
		  }
	  }
	  
	  return true;
  }
  
  protected static void shift(byte[] array) {
	array[0] = (byte) (array[0] << 1);
	for (int i = 1 ; i < array.length ; i++ ) {
	    if ( (array[i] & 0x80) > 0) {
	    	array[i - 1] |= 0x01;
	    }
	    array[i] = (byte) (array[i] << 1);
	}
  }
  
  public static void main(String[] args) {
	  byte[] array = { (byte) 0x31, (byte) 0x32, (byte) 0x33, (byte) 0x34, (byte) 0x35, (byte) 0x09, (byte) 0x17};
	  DataByteInputStream str = new DataByteInputStream(array);
	  System.out.println(str.isValid());
  }
  
}
