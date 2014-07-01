<%@ page session="false"%>
<%@taglib uri="http://www.springframework.org/tags/form" prefix="form"%>
<%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>



<div id="content">
	<div class="CSSTableGenerator">
		<table>
			<tr>
				<td>Sensor Data Id</td>
				<td>Sensor</td>
				<td>Date</td>
				<td>Danger Level</td>
				<td>State</td>
				<td>Data Type</td>
			</tr>

			<c:forEach var="data" items="${sensorDatas}" varStatus="loop">
				<c:if test="${data.dangerLevel == 'NONE' }">
					<tr style="background-color: #adff2f !important;">
						<td>${data.idSensorData}</td>
						<td>${idSensors[loop.index]}</td>
						<td>${data.timeStamp}</td>
						<td>${data.dangerLevel}</td>
						<td>${data.state}</td>
						<td>${data.type}</td>
					</tr>
				</c:if>
				<c:if test="${data.dangerLevel == 'DANGER' }">
					<tr style="background-color: #ff8c00 !important;">
						<td>${data.idSensorData}</td>
						<td>${idSensors[loop.index]}</td>
						<td>${data.timeStamp}</td>
						<td>${data.dangerLevel}</td>
						<td>${data.state}</td>
						<td>${data.type}</td>
					</tr>
				</c:if>
				<c:if test="${data.dangerLevel == 'ALARM' }">
					<tr style="background-color: red !important;">
						<td>${data.idSensorData}</td>
						<td>${idSensors[loop.index]}</td>
						<td>${data.timeStamp}</td>
						<td>${data.dangerLevel}</td>
						<td>${data.state}</td>
						<td>${data.type}</td>
					</tr>
				</c:if>
			</c:forEach>

		</table>
	</div>

</div>