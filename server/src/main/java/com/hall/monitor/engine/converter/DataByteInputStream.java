package com.hall.monitor.engine.converter;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Klasa tworzaca stream dla konwersji bajtow na obiekt protokolu. Nazwy funkcji odpowiadaja typom C++.
 * @author Marcin Serwach
 *
 */
public class DataByteInputStream
{
  
  /** bufor */
  private ByteBuffer buffer;
  
  /**
   * Konstruktor
   * @param bytes tablica bajtow do odczytu
   */
  public DataByteInputStream(byte bytes[])
  {
    buffer = ByteBuffer.wrap(bytes);
    buffer.order(ByteOrder.LITTLE_ENDIAN);
  }
  
  /**
   * Odczytuje byte (C++: int8_t)
   * @return byte (C++: int8_t)
   */
  public byte readInt8() {
    return buffer.get();
  }
  
  /**
   * Odczytuje char (C++: uint8_t)
   * @return char (C++: uint8_t)
   */
  public char readUInt8() {
    return (char) (buffer.get() & 0xFF);
  }
  
  /**
   * Odczytuje short (C++: int16_t)
   * @return short (C++: int16_t)
   */
  public short readInt16() {
    return buffer.getShort();
  }
  
  /**
   * Odczytuje char (C++: uint16_t)
   * @return char (C++: uint16_t)
   */
  public char readUInt16() {
    return buffer.getChar();
  }
  
  /**
   * Odczytuje int (C++: int32_t)
   * @return int (C++: int32_t)
   */
  public int readInt32() {
    return buffer.getInt();
  }
  
  /**
   * Odczytuje long (C++: uint32_t)
   * @return long (C++: uint32_t)
   */
  public long readUInt32() {
    return (char) (buffer.getInt() & 0xFFFFFFFFL);
  }
  
  /**
   * Odczytuje long (C++: int64_t)
   * @return long (C++: int64_t)
   */
  public long readInt64() {
    return buffer.getLong();
  }
  
  /**
   * Odczytuje float (C++: float)
   * @return float (C++: float)
   */
  public float readFloat32() {
    return buffer.getFloat();
  }
  
  /**
   * Odczytuje double (C++: double)
   * @return double (C++: double)
   */
  public double readDouble64() {
    return buffer.getDouble();
  }
  
  /**
   * Ustawia wskaznik odczytu
   * @param newPosition nowa pozycja
   */
  public void setPosition(int newPosition) {
    buffer.position(newPosition);
  }
  
  /**
   * zwraca pozycje odczytu
   * @return pozycja odczytu
   */
  public int getPosiotion() {
    return buffer.position();
  }

  /**
   * Sprawdza czy CRC jest poprawny
   * @return true jesli poprawny
   */
  public boolean isValidCRC()
  {
    return CRCHelper.isValid(buffer.array().clone());
  }
}
