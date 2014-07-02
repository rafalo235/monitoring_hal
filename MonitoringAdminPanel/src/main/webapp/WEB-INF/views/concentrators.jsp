<%@ page session="false"%>
<%@taglib uri="http://www.springframework.org/tags/form" prefix="form"%>
<%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>



<div id="content">
	<div class="CSSTableGenerator">
		<table>
			<tr>
				<td>Concentrator ID</td>
				<td>Number of sensors</td>
				<td>Action</td>
			</tr>

			<c:forEach var="concentrator" items="${concentrators}" varStatus="loop">
				<tr>
					<td onclick="window.location='monitorDatas?idConcentrator=${concentrator.idConcentrator}'">${concentrator.idConcentrator}
					</td>
					<td onclick="window.location='monitorDatas?idConcentrator=${concentrator.idConcentrator}'">${sizes[loop.index]}
					</td>
					
					<td style="width: 7%;">
						<input type="button"
							onclick="location.href='concentrators/delete?idConcentrator=${concentrator.idConcentrator}'"
							value="Delete">
						<input type="button"
							onclick="location.href='concentrators/config?idConcentrator=${concentrator.idConcentrator}'"
							value="Configure">
					</td>
				</tr>
			</c:forEach>

		</table>
	</div>
	
	<input type="submit" onclick="location.href='concentrators/add'" id="add-new-concentrator" name="add" value="Add" />


</div>