package observer;

import java.util.Observable;
import java.util.Observer;

public class WeatherData2 extends Observable {
	private float temperature;
	private float humidity;
	private float pressure;
	
	public WeatherData2() {
		
	}
	
	public void measurementsChanged() {
		setChanged();
		notifyObservers();
	}
	
	public void setMeasurements(float t, float h, float p) {
		this.temperature = t;
		this.humidity = h;
		this.pressure = p;
		measurementsChanged();
	}
	
	public float getTemperature() {
		return temperature;
	}
	
	public float getHumidity() {
		return humidity;
		
	}
	
	public float getPressure() {
		return pressure;
	}
}
