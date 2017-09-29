package nl.han.weatherboys.dto;

import io.swagger.annotations.ApiModelProperty;

public class NowResponse {

    @ApiModelProperty(notes = "The unix timestamp in seconds of the gateway and the jorgapi. This timenunit should " +
            "be used when sendiding measurements.",
            required = true, example = "1506697978", dataType = "long")
    public long time;

    public NowResponse(long time) {
        this.time = time;
    }
}
