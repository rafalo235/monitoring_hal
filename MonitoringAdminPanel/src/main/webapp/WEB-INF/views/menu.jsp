<%@page language="Java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>

<link href="<c:url value="/resources/css/menu.css"/>" rel="stylesheet" type="text/css" />

<c:set var="contextPath" value="${pageContext.request.contextPath}"/>
<ul class="menu">
  <li><a href="${contextPath}/" class="active"><span>Home</span></a></li>
  <li><a href="${contextPath}/companies"><span>Data</span></a></li>

</ul>