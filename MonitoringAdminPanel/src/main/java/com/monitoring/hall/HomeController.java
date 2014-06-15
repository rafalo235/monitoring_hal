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

import com.monitoring.hall.beans.Company;
import com.monitoring.hall.beans.Concentrator;
import com.monitoring.hall.beans.Hall;
import com.monitoring.hall.services.PersistenceService;


@Controller
public class HomeController {
	
	@Autowired
	private PersistenceService persistence;
	
	private List<Company> companies; 
	private List<Hall> halls; 
	private List<Concentrator> concentrators; 
	
	private int chosenCompany;
	private int chosenHall;
	
	@RequestMapping(value = "/", method = RequestMethod.GET)
	public String home(Model model) {
		
		// w razie pustej bazy odkomentowac i puscic. Nie wiem czemu, ale aplikacja nie dziala, gdy baza jest pusta
		
	/*	Company c = new Company("Firma1", "Lodz");
		persistence.addCompany(c);
		Hall h = new Hall("hala1", "Kosciuszki 22", c);
		persistence.addHall(h);
		Concentrator conc = new Concentrator(h);
		persistence.addConcentrator(conc);
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
		model.addAttribute("sizes",sizes);
		
		
		Company newCompany = new Company();
		model.addAttribute("newCompany", newCompany);
		
	
		
		return "companies";
	}
	
	@RequestMapping(value = "/companies/add", method = RequestMethod.POST)
	public String addCompany(@ModelAttribute Company newCompany,
			BindingResult result) {

		if (result.hasErrors()) {
			System.out.println(result.getFieldErrors());
		} else {
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
		model.addAttribute("sizes",sizes);
		
		Hall newHall = new Hall();
		model.addAttribute("newHall", newHall);
		
		return "halls";
	}
	
	@RequestMapping(value = "/halls/add", method = RequestMethod.POST)
	public String addHall(@ModelAttribute Hall newHall,
			BindingResult result) {

		if (result.hasErrors()) {
			System.out.println(result.getFieldErrors());
		} else {
			for (Company c : companies) {
				if(c.getIdCompany()==chosenCompany) newHall.setCompany(c);
			}
			persistence.addHall(newHall);
		}
		return "redirect:/halls?idCompany="+chosenCompany;
	}
	
	@RequestMapping(value = "/halls/delete", method = RequestMethod.GET)
	public String deleteHall(@RequestParam int idHall, Model model) {
		
		persistence.removeHall(idHall);
		
		return "redirect:/halls?idCompany="+chosenCompany;
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
		model.addAttribute("sizes",sizes);
		
		return "concentrators";
	}
	
	@RequestMapping(value = "/concentrators/add", method = RequestMethod.GET)
	public String addConcentrator() {
			
		Concentrator c = new Concentrator();
			for (Hall h : halls) {
				if(h.getIdHall()==chosenHall) {
					persistence.addConcentrator(new Concentrator(h));
				}
			}

		return "redirect:/concentrators?idHall="+chosenHall;
	}
	
	@RequestMapping(value = "/concentrators/delete", method = RequestMethod.GET)
	public String deleteConcentrator(@RequestParam int idConcentrator, Model model) {
		
		persistence.removeConcentrator(idConcentrator);
		
		return "redirect:/concentrators?idHall="+chosenHall;
	}
	
	
	
}
