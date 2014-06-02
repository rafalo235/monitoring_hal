package com.hall.monitor.database;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class DBUtil
{
  public static String toSHA1(String str) {
    byte convertme[] = str.getBytes();
    MessageDigest md = null;
    try {
      md = MessageDigest.getInstance("SHA-1");
    }
    catch (NoSuchAlgorithmException e) {
      e.printStackTrace();
    }
    byte res[] = md.digest(convertme);
    return byteArrayToHexString(res);
  }
  
  private static String byteArrayToHexString(byte b[]) {
    String result = "";
    for (int i = 0; i < b.length; i++) {
      result += Integer.toString((b[i] & 0xff) + 0x100, 16).substring(1);
    }
    return result;
  }
}
