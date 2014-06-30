package com.hall.monitor.engine.converter;

import java.security.InvalidParameterException;

public class CRCHelper
{
  /**
   * Zwraca CRC - wykorzystywane do pakietow wysylanych z serwera
   * @param data
   * @return
   */
  public static char countCrc16(byte data[]) {
    
    byte[] divisor =
    { (byte) 0xFE, (byte) 0xEF, (byte) 0x80 }; // TODO
    char crc = 0x0000;
    
    xorBytes(data, data.length - 2, divisor);
    crc = (char) ((data[1] << 8) + data[0]);
    
    return crc;
  }
  
  /**
   * Sprawdza CRC - na koncu pakietu jest suma kontrolna, wiec idzie zgodnie z algorytmem z wiki
   * @param data
   * @return
   */
  public static boolean isValid(byte data[]) {
    
    //////////////////////////////////////////////////////////////////// RAFAL TUTAJ ////
    byte[] divisor =
    { (byte) 0xFE, (byte) 0xEF, (byte) 0x80 }; // TODO
    
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
}
