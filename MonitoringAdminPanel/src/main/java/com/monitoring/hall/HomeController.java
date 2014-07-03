package com.monitoring.hall;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import com.googlecode.charts4j.Color;
import com.googlecode.charts4j.Data;
import com.googlecode.charts4j.GCharts;
import com.googlecode.charts4j.Plots;
import com.googlecode.charts4j.XYLine;
import com.googlecode.charts4j.XYLineChart;
import com.monitoring.hall.beans.Company;
import com.monitoring.hall.beans.Concentrator;
import com.monitoring.hall.beans.ConcentratorConf;
import com.monitoring.hall.beans.Hall;
import com.monitoring.hall.beans.MonitorData;
import com.monitoring.hall.beans.Sensor;
import com.monitoring.hall.beans.SensorConf;
import com.monitoring.hall.beans.SensorData;
import com.monitoring.hall.protocol.EConfigurationType;
import com.monitoring.hall.protocol.EValueType;
import com.monitoring.hall.protocol.SData;
import com.monitoring.hall.services.PersistenceService;
import com.monitoring.util.SensorDataComparator;

@Controller
public class HomeController
{
  
  @Autowired
  private PersistenceService persistence;
  
  private List<Company>      companies;
  private List<Hall>         halls;
  private List<Concentrator> concentrators;
  
  private int                chosenCompany;
  private int                chosenHall;
  
  @RequestMapping(value = "/", method = RequestMethod.GET)
  public String home(Model model) {
    
    // w razie pustej bazy odkomentowac i puscic. Nie wiem czemu, ale aplikacja
    // nie dziala, gdy baza jest pusta
    
    /*
     * Company c = new Company("Firma1", "Lodz"); persistence.addCompany(c);
     * Hall h = new Hall("hala1", "Kosciuszki 22", c); persistence.addHall(h);
     * Concentrator conc = new Concentrator(h);
     * persistence.addConcentrator(conc);
     */
    
    return "home";
  }
  
  @RequestMapping(value = "/companies", method = RequestMethod.GET)
  public String companies(Model model) {
    
    companies = persistence.listCompany();
    model.addAttribute("companies", companies);
    
    List<Integer> sizes = new ArrayList<Integer>();
    for (Company c : companies) {
      sizes.add(c.getHalls().size());
    }
    model.addAttribute("sizes", sizes);
    
    Company newCompany = new Company();
    model.addAttribute("newCompany", newCompany);
    
    return "companies";
  }
  
  @RequestMapping(value = "/companies/add", method = RequestMethod.POST)
  public String addCompany(@ModelAttribute Company newCompany,
      BindingResult result) {
    
    if (result.hasErrors()) {
      System.out.println(result.getFieldErrors());
    }
    else {
      persistence.addCompany(newCompany);
    }
    return "redirect:/companies";
  }
  
  @RequestMapping(value = "/companies/delete", method = RequestMethod.GET)
  public String deleteCompany(@RequestParam int idCompany, Model model) {
    
    persistence.removeCompany(idCompany);
    
    return "redirect:/companies";
  }
  
  @RequestMapping(value = "/halls", method = RequestMethod.GET)
  public String companies(@RequestParam int idCompany, Model model) {
    chosenCompany = idCompany;
    halls = persistence.getHalls(idCompany);
    model.addAttribute("halls", halls);
    
    List<Integer> sizes = new ArrayList<Integer>();
    for (Hall h : halls) {
      sizes.add(h.getConcentrators().size());
    }
    model.addAttribute("sizes", sizes);
    
    Hall newHall = new Hall();
    model.addAttribute("newHall", newHall);
    
    return "halls";
  }
  
  @RequestMapping(value = "/halls/add", method = RequestMethod.POST)
  public String addHall(@ModelAttribute Hall newHall, BindingResult result) {
    
    if (result.hasErrors()) {
      System.out.println(result.getFieldErrors());
    }
    else {
      for (Company c : companies) {
        if (c.getIdCompany() == chosenCompany)
          newHall.setCompany(c);
      }
      persistence.addHall(newHall);
    }
    return "redirect:/halls?idCompany=" + chosenCompany;
  }
  
  @RequestMapping(value = "/halls/delete", method = RequestMethod.GET)
  public String deleteHall(@RequestParam int idHall, Model model) {
    
    persistence.removeHall(idHall);
    
    return "redirect:/halls?idCompany=" + chosenCompany;
  }
  
  @RequestMapping(value = "/concentrators", method = RequestMethod.GET)
  public String halls(@RequestParam int idHall, Model model) {
    chosenHall = idHall;
    concentrators = persistence.getConcentrators(idHall);
    model.addAttribute("concentrators", concentrators);
    
    List<Integer> sizes = new ArrayList<Integer>();
    for (Concentrator c : concentrators) {
      sizes.add(c.getSensors().size());
    }
    model.addAttribute("sizes", sizes);
    
    return "concentrators";
  }
  
  @RequestMapping(value = "/concentrators/add", method = RequestMethod.GET)
  public String addConcentrator() {
    
    for (Hall h : halls) {
      if (h.getIdHall() == chosenHall) {
        persistence.addConcentrator(new Concentrator(h));
      }
    }
    
    return "redirect:/concentrators?idHall=" + chosenHall;
  }
  
  @RequestMapping(value = "/concentrators/delete", method = RequestMethod.GET)
  public String deleteConcentrator(@RequestParam int idConcentrator, Model model) {
    
    persistence.removeConcentrator(idConcentrator);
    
    return "redirect:/concentrators?idHall=" + chosenHall;
  }
  
  @RequestMapping(value = "/concentrators/config", method = RequestMethod.GET)
  public String showConcentratorConfiguration(@RequestParam int idConcentrator, Model model) {
	  
	  ConcentratorConf conf = persistence.getConcentratorConf(idConcentrator);
	  model.addAttribute("concentratorConf", conf);
	  return "concentratorConfiguration";
  }
  
  @RequestMapping(value = "/concentrators/config/modify", method = RequestMethod.GET)
  public String modifySensorConfiguration(
		  @RequestParam int sensorConfId,
		  @RequestParam String value,
		  Model model) {
	  
	  SensorConf sensorConf = persistence.getSensorConf(sensorConfId);
	  sensorConf.setDataStr(value);
	  persistence.setSensorConf(sensorConf);

	  return "redirect:/concentrators/config?idConcentrator=" + 
	  			sensorConf
	  				.getConcentratorConf()
	  				.getConcentrator()
	  				.getIdConcentrator();
  }
  
  @RequestMapping(value = "/monitorDatas", method = RequestMethod.GET)
  public String monitorData(@RequestParam int idConcentrator, Model model) {
    
    List<MonitorData> monitorDatas = persistence
        .getMonitorDatas(idConcentrator);
    model.addAttribute("monitorDatas", monitorDatas);
    
    return "monitorDatas";
  }
  
  @RequestMapping(value = "/sensorDatas", method = RequestMethod.GET)
  public String sensorDatas(@RequestParam int idMonitorData, Model model) {
    
    List<SensorData> sensorDatas = persistence.getSensorDatas(idMonitorData);
    model.addAttribute("sensorDatas", sensorDatas);
    
    List<Integer> idSensors = new ArrayList<Integer>();
    for (SensorData sensorData : sensorDatas) {
      idSensors.add(sensorData.getSensor().getIdSensor());
    }
    
    model.addAttribute("idSensors", idSensors);
    
    return "sensorDatas";
  }
  
  @RequestMapping(value = "/statistics", method = RequestMethod.GET)
  public String statistics(Model model) {
    
    List<Concentrator> concentrators = persistence.listConcentrators(); 
    
    List<String> chartsUrls = new ArrayList<String>();
    
    for (Concentrator c : concentrators) {
      ConcentratorConf coConf = persistence.getConcentratorConf(c.getIdConcentrator());
      Set<SensorConf> seConf = coConf.getSensorConf();
    	
      for (Sensor sensor : c.getSensors()) {
          List<Double> y = new ArrayList<Double>();
          List<Long> x = new ArrayList<Long>();
          ArrayList<SensorData> records = new ArrayList<SensorData>(sensor.getSensorDatas());
          Collections.sort(records, new SensorDataComparator());
          
    	  
          for (SensorData record : records) {
        	  try {
        		  SData data = convert(record.getType(), record.getDataStr());
        		  char buf1 = (Character) data.getValue();
        		  double val1 = buf1;
        		  
        		  y.add( new Double( val1 ) );
        		  x.add( record.getTimeStamp().getTime() );
        		  
        	  } catch (NumberFormatException e) {
        		  System.err.println("Wrong data record format");
        	  }
          }
          
          for (int j = 0 ; j < x.size() - 50 ; j++) {
        	  x.remove(0);
        	  y.remove(0);
          }

          long minX = -1;
          for (long l : x) {
        	  if (minX == -1 || minX > l) {
    			  minX = l;
    		  }
          }		  
          
          for (int i = 0 ; i < x.size() ; i++ ) {
        	  x.set(i, x.get(i) - minX);
        	  x.set(i, x.get(i) / 1000);
          }
          

          XYLine line = Plots.newXYLine(Data.newData(x), Data.newData(y), Color.BLACK);
          XYLineChart lineChart = GCharts.newXYLineChart(line);

          lineChart.setTitle(
        		  c.getHall().getHallName() + " - Concentrator " + c.getIdConcentrator() + " - Sensor " + sensor.getIdSensor(),
        		  Color.BLACK,
        		  18);
          lineChart.setSize(640, 320);
          
          Double begin = null;
          Double end = null;
          
          for (SensorConf conf : seConf) {
        	  if (conf.getSensor().getIdSensor() == sensor.getIdSensor()) {
        		  try {
        			  SData data2 = convert(conf.getType(), conf.getDataStr());
        			  int buf2 = (Integer) data2.getValue();
        			  double val2 = buf2;
        			  
	                  if (conf.getConfigType() == EConfigurationType.DANGER_LEVEL) {
	                	  begin = new Double( val2 );
	                  } else if (conf.getConfigType() == EConfigurationType.ALARM_LEVEL) {
	                	  end = new Double( val2 );
	                  }
        		  } catch (NumberFormatException e) {
        			  System.err.println("Wrong data record format");
        			  break;
        		  }
        	  }
          }
          if (begin != null && end != null) {
        	  lineChart.addHorizontalRangeMarker(begin.doubleValue(), end.doubleValue(), Color.ORANGE);
        	  lineChart.addHorizontalRangeMarker(end.doubleValue(), Data.MAX_VALUE, Color.RED);
          }
          if (!x.isEmpty()) {
              chartsUrls.add(lineChart.toURLString());
          }
	  } 
      
    }
    
    model.addAttribute("charts", chartsUrls);
    
    return "statistics";
  }
  
  private SData convert(EValueType type, String str) {
	    switch (type)
	    {
	    case DOUBLE_64:
	      return new SData(type, Double.valueOf(str));
	    case FLOAT_32:
	      return new SData(type, Float.valueOf(str));
	    case INT_16:
	      return new SData(type, Short.valueOf(str));
	    case INT_32:
	      return new SData(type, Integer.valueOf(str));
	    case INT_64:
	      return new SData(type, Long.valueOf(str));
	    case INT_8:
	      return new SData(type, str.charAt(0));
	    case UINT_64:
	      return new SData(type, Long.valueOf(str));
	    case UINT_32:
	      return new SData(type, Long.valueOf(str));
	    case UINT_16:
	      return new SData(type, Character.valueOf(str.charAt(0)));
	    case UINT_8:
	      return new SData(type, str.charAt(0));
	    case VOID:
	    default:
	      return SData.VOID;
	    }
	  }
  
}

