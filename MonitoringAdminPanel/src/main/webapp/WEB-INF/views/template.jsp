<%@ taglib uri="http://tiles.apache.org/tags-tiles" prefix="tiles"%>
<%@page import="javax.swing.text.Document"%>
<%@page language="Java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib uri="http://www.springframework.org/tags/form" prefix="form"%>
<%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
<link href="<c:url value="/resources/css/main.css"/>" rel="stylesheet" type="text/css" />
<script src="<c:url value="/resources/javascript/jquery-1.11.1.min.js"/>"></script>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title><tiles:insertAttribute name="title" ignore="true" /></title>
</head>
<body>
		<div id="title"><tiles:insertAttribute name="title" /></div>	
		<div><tiles:insertAttribute name="header" /></div>	
		<div><tiles:insertAttribute name="menu" /></div>
		<div><tiles:insertAttribute name="body" /></div>	

</body>
</html>