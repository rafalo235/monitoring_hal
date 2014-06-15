<%@ page session="false"%>
<%@taglib uri="http://www.springframework.org/tags/form" prefix="form"%>
<%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>



<script type="text/javascript">
	$(document).ready(function() {
		
		$("#add-new-hall").click(function() {
		$("#add-new-hall").hide();
		$(".CSSTableGenerator").hide();
		$("#formm").css('display', 'table');
		});
	});
	
	</script>

<div id="content">
	<div class="CSSTableGenerator">
		<table>
			<tr>
				<td>Hall Name</td>
				<td>Hall Address</td>
				<td>Number of concentrators</td>
				<td>Delete</td>
			</tr>

			<c:forEach var="hall" items="${halls}" varStatus="loop">
				<tr>
					<td
						onclick="window.location='concentrators?idHall=${hall.idHall}'">${hall.hallName}
					</td>
					<td
						onclick="window.location='concentrators?idHall=${hall.idHall}'">${hall.hallAddress}</td>
						<td
						onclick="window.location='halls?idCompany=${hall.idHall}'">${sizes[loop.index]}
					</td>
					<td style="width: 7%;"><input type="button"
						onclick="location.href='halls/delete?idHall=${hall.idHall}'"
						value="Delete"></td>
				</tr>
			</c:forEach>

		</table>
	</div>
	
	<input type="submit" id="add-new-hall" name="add" value="Add" />

	<form:form id="formm" method="post" action="halls/add"
		modelAttribute="newHall" style="display: none; margin: 0 auto;">
		Add new hall:
		<table>
			<tr>
				<td>Name:</td>
				<td><form:input type="text" path="hallName" name="hallName"
						title="Insert name" size="15" maxlength="120" /></td>
			</tr>
			<tr>
				<td>Address:</td>
				<td><form:input type="text" path="hallAddress" name="hallAddress"
						title="Insert address" size="15" maxlength="120" /></td>
			</tr>
			<form:input type="hidden" path="idHall" name="idHall" />
		</table>
		<input type="submit" id="add" name="add" value="Add" />

	</form:form>

</div>