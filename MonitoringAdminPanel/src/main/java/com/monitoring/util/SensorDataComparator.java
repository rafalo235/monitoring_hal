package com.monitoring.util;

import java.util.Comparator;

import com.monitoring.hall.beans.SensorData;

public class SensorDataComparator implements Comparator<SensorData> {

	@Override
	public int compare(SensorData o1, SensorData o2) {
		return o1.getTimeStamp().compareTo(o2.getTimeStamp());
	}

}
