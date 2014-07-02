package com.monitoring.hall;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import com.googlecode.charts4j.Color;
import com.googlecode.charts4j.GCharts;
import com.googlecode.charts4j.PieChart;
import com.googlecode.charts4j.Slice;
import com.monitoring.hall.beans.Company;
import com.monitoring.hall.beans.Concentrator;
import com.monitoring.hall.beans.ConcentratorConf;
import com.monitoring.hall.beans.Hall;
import com.monitoring.hall.beans.MonitorData;
import com.monitoring.hall.beans.SensorConf;
import com.monitoring.hall.beans.SensorData;
import com.monitoring.hall.services.PersistenceService;

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
    
    halls = persistence.listHalls();
    List<SensorData> sensorDatas = persistence.listSensorDatas();
    
    List<String> chartsUrls = new ArrayList<String>();
    
    for (Hall hall : halls) {
      int none = 0;
      int danger = 0;
      int alarm = 0;
      
      for (SensorData sensorData : sensorDatas) {
        if (hall.getIdHall() == sensorData.getMonitorData().getConcentrator()
            .getHall().getIdHall()) 
        {
          if (sensorData.getDangerLevel().name() == "NONE")
          {
            none++;
          }
          else if (sensorData.getDangerLevel().name() == "DANGER")
          {
            danger++;
          }
          else
          {
            alarm++;
          }
        }
      }
      
      int total = none + danger + alarm;
      int percentNone = 0;
      
      int percentDanger = 0;
      int percentAlarm = 0;
      if (total != 0) {
        percentNone = (int) (none * 100 / total);
        percentDanger = (int) (danger * 100 / total);
        percentAlarm = (int) (alarm * 100 / total);
      }
      Slice s1 = Slice.newSlice(percentNone, Color.newColor("adff2f"),
          "Positive", "Positive value");
      Slice s2 = Slice.newSlice(percentDanger, Color.newColor("ff8c00"),
          "Danger", "Danger");
      Slice s3 = Slice.newSlice(percentAlarm, Color.newColor("ff0000"),
          "Alarm", "Alarm");
      
      PieChart pieChart = GCharts.newPieChart(s1, s2, s3);
      pieChart.setTitle(hall.getHallName(), Color.BLACK, 18);
      pieChart.setSize(640, 320);
      pieChart.setThreeD(true);
      
      chartsUrls.add(pieChart.toURLString());
      
    }
    
    model.addAttribute("charts", chartsUrls);
    
    return "statistics";
  }
  
}
