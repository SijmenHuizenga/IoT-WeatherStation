package nl.han.weatherboys.dto;

import io.swagger.annotations.ApiModelProperty;

public class PutData {

    @ApiModelProperty(notes = "The unix timestamp in seconds that represents the moment the measurements are taken",
            required = true, example = "1506697978", dataType = "long")
    public Long timestamp;

    @ApiModelProperty(notes = "A float measurment of the air humidity between 0.0 and 80.0 percent.",
            required = true, example = "55.48", dataType = "float")
    public Float humidity;

    @ApiModelProperty(notes = "A float measurment of the light between 0.0 and 100.0 percent.",
            required = true, example = "78.6", dataType = "float")
    public Float brightness;

    @ApiModelProperty(notes = "A float measurment of the air temperature between 10.0 and 85.0 degrees celsius.",
            required = true, example = "23.6", dataType = "float")
    public Float temperature;

    @ApiModelProperty(notes = "A float measurment of the surrounding pressure. Details to be determand.",
            required = true, example = "10.6", dataType = "float")
    public Float pressure;

    public PutData() {
    }

    public PutData(Float humidity, Float brightness, Float temperature, Float pressure) {
        this.humidity = humidity;
        this.brightness = brightness;
        this.temperature = temperature;
        this.pressure = pressure;
    }

    @Override
    public String toString() {
        return "{ hum:" + humidity +
                ", bri:" + brightness +
                ", tem:" + temperature +
                ", pre:" + pressure + "}";
    }
}
