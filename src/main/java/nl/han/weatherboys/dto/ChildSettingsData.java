package nl.han.weatherboys.dto;

import io.swagger.annotations.ApiModelProperty;
import nl.han.weatherboys.util.FMax;

import javax.validation.constraints.Min;

public class ChildSettingsData {

    @ApiModelProperty(notes = "Treshhold green: When the temparture measured by the child is lower than this value " +
            "than the green led lights up. When the measured temperatue is higher than than this value but " +
            "lower than the `treshhold-red` value than the orange led lights up. This value must be smaller than the " +
            "'r' value and below 100. Negative values are not allowed.", required = true, example = "18.5f")
    @Min(0)
    @FMax(100)
    public float g;

    @ApiModelProperty(notes="Treshhol red: when the termperature measured by the child is higher than this value " +
            "than the red led lights up. This value must be bigger than the 'g' value and not be bigger than "+
            "100. Negative values are not allowed.", required = true, example = "23.6f")
    @Min(0)
    @FMax(100)
    public float r;

    @Override
    public String toString() {
        return "ChildSettingsData{" +
                "g=" + g +
                ", r=" + r +
                '}';
    }
}
