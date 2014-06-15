<%@ page session="false"%>
<%@taglib uri="http://www.springframework.org/tags/form" prefix="form"%>
<%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>



<script type="text/javascript">
	$(document).ready(function() {
		
		$("#add-new-company").click(function() {
		$("#add-new-company").hide();
		$(".CSSTableGenerator").hide();
		$("#formm").css('display', 'table');
		});
	});
	
	</script>

<div id="content">
	<div class="CSSTableGenerator">
		<table>
			<tr>
			
				<td>Name</td>
				<td>Address</td>
				<td>Number of halls</td>
				<td>Delete</td>
			</tr>

			<c:forEach var="company" items="${companies}" varStatus="loop">
				<tr>
					<td
						onclick="window.location='halls?idCompany=${company.idCompany}'">${company.companyName}
					</td>
					<td
						onclick="window.location='halls?idCompany=${company.idCompany}'">${company.companyAddress}</td>
						<td
						onclick="window.location='halls?idCompany=${company.idCompany}'">${sizes[loop.index]}
					</td>
					
					<td style="width: 7%;"><input type="button"
						onclick="location.href='companies/delete?idCompany=${company.idCompany}'"
						value="Delete"></td>
				</tr>
			</c:forEach>

		</table>
	</div>
	
	<input type="submit" id="add-new-company" name="add" value="Add" />

	<form:form id="formm" method="post" action="companies/add"
		modelAttribute="newCompany" style="display: none; margin: 0 auto;">
		Add new company:
		<table>
			<tr>
				<td>Name:</td>
				<td><form:input type="text" path="companyName" name="companyName"
						title="Insert name" size="15" maxlength="120" /></td>
			</tr>
			<tr>
				<td>Address:</td>
				<td><form:input type="text" path="companyAddress" name="companyAddress"
						title="Insert address" size="15" maxlength="120" /></td>
			</tr>
			<form:input type="hidden" path="idCompany" name="idCompany" />

		</table>
		<input type="submit" id="add" name="add" value="Add" />

	</form:form>

</div>