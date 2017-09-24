package nl.han.weatherboys.dto;

public class PutData {

    public Float humidity, brightness, temperature, pressure;

    public PutData() {
    }

    public PutData(Float humidity, Float brightness, Float temperature, Float pressure) {
        this.humidity = humidity;
        this.brightness = brightness;
        this.temperature = temperature;
        this.pressure = pressure;
    }
}
