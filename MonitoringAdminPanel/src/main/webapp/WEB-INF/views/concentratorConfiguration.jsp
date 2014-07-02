<%@page language="Java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib uri="http://www.springframework.org/tags/form" prefix="form"%>
<%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>

<script type="text/javascript">
<!--

function saveSensorConfiguration(configId) {
	var v = document.getElementById('conf' + configId).value;
	window.location = '/hall/concentrators/config/modify?sensorConfId=' + configId
			+ '&value=' + v;
} 

//-->
</script>


<div id="content">
	<div>Last modification: ${concentratorConf.timeStamp}</div>

	<div class="CSSTableGenerator">
		<table>
			<tr>
				<th>Sensor ID</th>
				<th>Configuration Type</th>
				<th>Data Type</th>
				<th>Value</th>
				<th>Action</th>
			</tr>
			<c:forEach var="conf" items="${concentratorConf.sensorConf}">
				<tr>
					<td>${conf.sensor.idSensor}</td>
					<td>${conf.configType}</td>
					<td>${conf.type}</td>
					<td><input id="conf${conf.idSensorConf}" type="text" value="${conf.dataStr}"></td>
					<td><button onclick="saveSensorConfiguration(${conf.idSensorConf})">Save</button></td>
				</tr>
			</c:forEach>
		</table>
	</div>
</div>
