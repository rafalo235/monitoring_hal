package com.hall.monitor.engine.sms;

import java.text.MessageFormat;
import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.apache.commons.mail.DefaultAuthenticator;
import org.apache.commons.mail.Email;
import org.apache.commons.mail.EmailException;
import org.apache.commons.mail.SimpleEmail;

import com.hall.monitor.database.data.Concentrator;
import com.hall.monitor.database.data.Hall;

/**
 * Obsluguje smsy
 * 
 * @author Marcin Serwach
 * 
 */
public class SMSManager
{
  private static final String SMS_PATTER      = "HALL: {0}\nADDRESS: {1}\nCONCENTRATOR: {2}\nWARNINGS: {3}\nALARMS: {4}";
  private ExecutorService     executorService = Executors.newFixedThreadPool(1);
  private static boolean      SENDING_SMS     = true;
  private static Logger       log             = Logger
                                                  .getLogger(SMSManager.class
                                                      .getSimpleName());
  
  /**
   * Tworzy tekst wiadomosci sms
   * 
   * @param concentrator
   *          koncentrator z ktorego pochodza warningi
   * @param warningSensorsIds
   *          identyfikatory czujnikow z warningami
   * @param alarmSensorsIds
   *          identyfikatory czujnikow z alarmami
   * @return tekst wiadomosci sms
   */
  private String getTextMessage(Concentrator concentrator,
      Set<Integer> warningSensorsIds, Set<Integer> alarmSensorsIds) {
    
    Hall hall = concentrator.getHall();
    String hallName = hall.getHallName();
    String hallAddress = hall.getHallAddress();
    int idConcentrator = concentrator.getIdConcentrator();
    
    return MessageFormat.format(SMS_PATTER, hallName, hallAddress,
        idConcentrator, warningSensorsIds.toString(),
        alarmSensorsIds.toString());
  }
  
  /**
   * Wysyla email do bramki PlusGSM, ktory powoduje wyslanie smsa
   * 
   * @param concentrator
   *          koncentrator z ktorego pochodza warningi
   * @param warningSensorsIds
   *          identyfikatory czujnikow z warningami
   * @param alarmSensorsIds
   *          identyfikatory czujnikow z alarmami
   */
  public void sendSms(final Concentrator concentrator,
      final Set<Integer> warningSensorsIds, final Set<Integer> alarmSensorsIds) {
    // TODO http://commons.apache.org/proper/commons-email/userguide.html
    
    executorService.execute(new Runnable()
    {
      
      @Override
      public void run() {
        try {
          Email email = new SimpleEmail();
          email.setHostName("smtp.googlemail.com");
          email.setSmtpPort(465);
          email.setAuthenticator(new DefaultAuthenticator("monitoringHal",
              "xswqaz12"));
          email.setSSLOnConnect(true);
          
          email.setFrom("monitoringHal@gmail.com");
          
          email.setSubject("Monitoring Hal");
          String txt = getTextMessage(concentrator, warningSensorsIds,
              alarmSensorsIds);
          email.setMsg(txt);
          email.addTo("+48785718645@text.plusgsm.pl");
          
          if (SENDING_SMS) {
            email.send();
          }
          else {
            log.log(
                Level.ALL,
                "SMS zostalby wyslany, ale nie chce byscie mi spamowali - "
                + "jesli chcecie wyslac sms to ustawcie com.hall.monitor.engine.sms.SMSManager.SENDING_SMS na true");
          }
        }
        catch (EmailException e) {
          // TODO Auto-generated catch block
          e.printStackTrace();
          
        }
      }
    });
    
  }
  
}
