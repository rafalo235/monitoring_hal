<%@ page session="false"%>
<%@taglib uri="http://www.springframework.org/tags/form" prefix="form"%>
<%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>



<div id="content">
	<div class="CSSTableGenerator">
		<table>
			<tr>
				<td>Monitor Data Id</td>
				<td>Id Package</td>
				<td>Receive Time</td>
				<td>Send Time</td>
				<td>Sensors Amount</td>
			</tr>

		<c:forEach var="data" items="${monitorDatas}" varStatus="loop">
				<tr>
					<td
						onclick="window.location='sensorDatas?idMonitorData=${data.idMonitorData}'">${data.idMonitorData}
					</td>
					<td
						onclick="window.location='sensorDatas?idMonitorData=${data.idMonitorData}'">${data.idPackage}
					</td>
					<td
						onclick="window.location='sensorDatas?idMonitorData=${data.idMonitorData}'">${data.receiveTime}
					</td>
					<td
						onclick="window.location='sensorDatas?idMonitorData=${data.idMonitorData}'">${data.sendTime}
					</td>
					<td
						onclick="window.location='sensorDatas?idMonitorData=${data.idMonitorData}'">${data.sensorsAmount}
					</td>
					
					
				</tr>
			</c:forEach>

		</table>
	</div>
	
</div>