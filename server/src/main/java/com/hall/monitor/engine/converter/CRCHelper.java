package com.hall.monitor.engine.converter;

import java.security.InvalidParameterException;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;

public class CRCHelper
{
  /**
   * Zwraca CRC - wykorzystywane do pakietow wysylanych z serwera
   * @param data tablica bajtow do zakodowania
   * @return tablica bajtow z obliczonego CRC16
   */
  public static byte[] countCrc16(byte data[]) {
    byte[] r = new byte[2];
	  
    byte[] divisor = generateDivisor();
    //{ (byte) 0xFE, (byte) 0xEF, (byte) 0x80 }; // TODO
    //char crc = 0x0000;
    
    xorBytes(data, data.length - 2, divisor);
    //crc = (char) ((data[1] << 8) + data[0]);
    r[0] = data[0];
    r[1] = data[1];
    
    return r;
  }
  
  /**
   * Sprawdza CRC - na koncu pakietu jest suma kontrolna, wiec idzie zgodnie z algorytmem z wiki
   * @param data tablica bajtow wraz z CRC na koncu
   * @return true jesli poprawny
   */
  public static boolean isValid(byte data[]) {
    
    //////////////////////////////////////////////////////////////////// RAFAL TUTAJ ////
    byte[] divisor = generateDivisor();
    //{ (byte) 0xFE, (byte) 0xEF, (byte) 0x80 }; // TODO
    
    xorBytes(data, data.length, divisor);
    
    for (byte b : data) {
      if (b != 0) {
        return false;
      }
    }
    
    return true;
  }
  
  private static void xorBytes(byte[] array, int length, byte[] divisor) {
    byte tmp;
    int pointer = length * 8;
    if (divisor.length < 3) {
      throw new InvalidParameterException("Divisor must have 3 bytes");
    }
    
    do {
      for (int i = 0; i < 8; i++) {
        if ((array[0] & 0x80) > 0) { // jeżeli najstarszy bit jest 1 oblicz XOR
          array[0] ^= divisor[0];
          array[1] ^= divisor[1];
          tmp = array[2];
          array[2] ^= divisor[2];
          array[2] &= 0x80;
          array[2] |= tmp & 0x7F;
        }
        shift(array); // przesuń bity danych o 1
        pointer--;
      }
    }
    while (pointer > 0);
  }
  
  private static void shift(byte[] array) {
    array[0] = (byte) (array[0] << 1);
    for (int i = 1; i < array.length; i++) {
      if ((array[i] & 0x80) > 0) {
        array[i - 1] |= 0x01;
      }
      array[i] = (byte) (array[i] << 1);
    }
  }
  
  private static byte[] generateDivisor() {
	  byte[] divisor = new byte[3];
	  
	  Calendar calendar = GregorianCalendar.getInstance();
	  calendar.setTime(new Date());
	  int day = calendar.get(Calendar.DAY_OF_MONTH);
	 
	  divisor[0] = (byte) (day | 0x80);
	  divisor[1] = (byte) (day | 0x80);
	  divisor[2] = (byte) 0x80;
	  
	  return divisor;
  }
  
  public static void main(String[] args) {
		DataByteOutputStream stream = new DataByteOutputStream(7);
		stream.writeUInt8((char) 0xFF);
		stream.writeUInt8((char) 0xFF);
		stream.writeUInt8((char) 0xFF);
		stream.writeUInt8((char) 0xFF);
		stream.writeUInt8((char) 0xFF);
		stream.writeByteArray(stream.calcCRC16());

		//byte[] array = { (byte) 0x31, (byte) 0x32, (byte) 0x33, (byte) 0x34, (byte) 0x35, (byte) 0x09, (byte) 0x17};
		DataByteInputStream str = new DataByteInputStream(stream.getBytes());
		System.out.println(str.isValidCRC());
  }
}
