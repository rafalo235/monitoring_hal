<%@ page session="false"%>
<%@taglib uri="http://www.springframework.org/tags/form" prefix="form"%>
<%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>



<div id="content">


		<c:forEach var="chart" items="${charts}" varStatus="loop">
				<div><img src="${charts[loop.index] }" /></div>
				<div style="height: 20px;"></div>
			</c:forEach>

	
	
</div>