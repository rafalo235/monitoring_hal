package com.hall.monitor.protocol;

/**
 * Klasa przechowywujace wartosci
 * @author Marcin Serwach
 *
 */
public class CData
{
  public static final Short VOID_VALUE = 0xFF;
  public static final CData VOID       = new CData(EValueType.VOID, VOID_VALUE);
  
  // EValueType type;
  /** typ wartosci w value */
  private EValueType        type;
  
  // UValue value;
  /** wartosc */
  private Object            value;
  
  /**
   * Konstruktor
   * @param type typ value
   * @param value wartosc
   */
  public CData(EValueType type, Object value)
  {
    super();
    this.type = type;
    this.value = value;
  }
  
  /**
   * Zwraca typ
   * @return typ
   */
  public EValueType getType() {
    return type;
  }
  
  /**
   * Zwraca wartosc
   * @return wartosc
   */
  public Object getValue() {
    return value;
  }
  
  /**
   * Zwraca rozmiar
   * @return rozmiar
   */
  public int getSize() {
    return (EValueType.getSizeBits() + type.getTypeSizeBits()) / 8;
  }
}
