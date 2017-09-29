package nl.han.weatherboys.dto;

import io.swagger.annotations.ApiModelProperty;
import nl.han.weatherboys.util.FMax;

import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;

public class PutData {

    @ApiModelProperty(notes = "The unix timestamp in seconds that represents the moment the measurements are taken",
            required = true, example = "1506697978", dataType = "long")
    @NotNull(message = "Timestamp is required")
    @Min(0)
    public Long timestamp;

    @ApiModelProperty(notes = "A float measurment of the air humidity between 0 and 80 percent.",
            required = true, example = "55.48", dataType = "float")
    @Min(0)
    @FMax(80)
    public Float humidity;

    @ApiModelProperty(notes = "A float measurment of the light between 0 and 100 percent.",
            required = true, example = "78.6", dataType = "float")
    @Min(0)
    @FMax(100)
    public Float brightness;

    @ApiModelProperty(notes = "A float measurment of the air temperature between 10 and 85 degrees celsius.",
            required = true, example = "23.6", dataType = "float")
    @Min(10)
    @FMax(85)
    public Float temperature;

    @ApiModelProperty(notes = "A float measurment of the surrounding pressure. Details to be determand.",
            required = true, example = "10.6", dataType = "float")
    @Min(0)
    public Float pressure;

    public PutData() {
    }

    public PutData(Long timestamp, Float humidity, Float brightness, Float temperature, Float pressure) {
        this.humidity = humidity;
        this.brightness = brightness;
        this.temperature = temperature;
        this.pressure = pressure;
        this.timestamp = timestamp;
    }

    @Override
    public String toString() {
        return "{ hum:" + humidity +
                ", bri:" + brightness +
                ", tem:" + temperature +
                ", pre:" + pressure + "}";
    }
}
