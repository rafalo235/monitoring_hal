package com.hall.monitor.engine.converter;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Klasa tworzaca stream dla konwersji obiektu pakietu na tablic bajtow. Nazwy funkcji odpowiadaja typom C++.
 * @author Marcin Serwach
 *
 */
public class DataByteOutputStream
{
  /** bufor */
  private ByteBuffer buffer;
  
  /** zapis void */
  public static final byte VoidValue = (byte) 0xFF;
  
  /**
   * Konstruktor
   * @param size rozmiar danych
   */
  public DataByteOutputStream(int size)
  {
    buffer = ByteBuffer.allocate(size);
    buffer.order(ByteOrder.LITTLE_ENDIAN);
  }
  
  /**
   * konwertuje byte (C++: int8_t)
   * @param value dana
   */
  public void writeInt8(byte value) {
    buffer.put(value);
  }
  
  /**
   * konwertuje char (C++: uint8_t)
   * @param value dana
   */
  public void writeUInt8(char value) {
    byte buf = (byte)(value & 0xFF);
    buffer.put(buf);
  }
  
  /**
   * konwertuje short (C++: int16_t)
   * @param value dana
   */
  public void writeInt16(short value) {
    buffer.putShort(value);
  }
  
  /**
   * konwertuje char (C++: uint16_t)
   * @param value dana
   */
  public void writeUInt16(char value) {
    buffer.putChar(value);
  }
  
  /**
   * konwertuje byte[] (C++: int8_t[])
   * @param array dane
   */
  public void writeByteArray(byte[] array) {
	  buffer.put(array);
  }
  
  /**
   * konwertuje int (C++: int32_t)
   * @param value dana
   */
  public void writeInt32(int value) {
    buffer.putInt(value);
  }
  
  /**
   * konwertuje long (C++: uint32_t)
   * @param value dana
   */
  public void writeUInt32(long value) {
    int buf = (int)(value & 0xFFFFFFFF);
    buffer.putInt(buf);
  }
  
  /**
   * konwertuje long (C++: int64_t)
   * @param value dana
   */
  public void writeInt64(long value) {
    buffer.putLong(value);
  }
  
  /**
   * konwertuje long (C++: uint64_t)
   * @param value dana
   */
  public void writeUInt64(long value) {
    buffer.putLong(value);
  }
  
  /**
   * konwertuje float (C++: float)
   * @param value dana
   */
  public void writeFloat32(float value) {
    buffer.putFloat(value);
  }
  
  /**
   * konwertuje double (C++: double)
   * @param value dana
   */
  public void writeDouble64(double value) {
    buffer.putDouble(value);
  }
  
  /**
   * dodaje dana symbolizujaca void
   */
  public void writeVoid(){
    buffer.put(VoidValue);
  }
  
  /**
   * Zmienia miejsce odczytu
   * @param newPosition pozycja
   */
  public void setPosition(int newPosition) {
    buffer.position(newPosition);
  }
  
  /**
   * 
   * @return pozycja odczytu
   */
  public int getPosiotion() {
    return buffer.position();
  }
  
  /**
   * Zwraca bajty
   * @return tablica bajtow
   */
  public byte[] getBytes(){
    return buffer.array();
  }
  
  /**
   * Oblicza crc16
   * @return tablica bajtow crc
   */
  public byte[] calcCRC16()
  {
    return CRCHelper.countCrc16(buffer.array().clone());
  }

}
