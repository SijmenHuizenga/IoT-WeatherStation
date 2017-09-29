package nl.han.weatherboys.dto;

import io.swagger.annotations.ApiModelProperty;

public class ChildLoginResponse {

    @ApiModelProperty(notes = "The childid number that is used to identify the child. " +
            "If the child already had an id than this field contains the existing identifier. " +
            "If this is the first time the child registers itsself than this field contains the " +
            "a new identifier.", required = true)
    public int id;

    @ApiModelProperty(notes = "The unix timestamp in seconds of the gateway and the jorgapi. This timenunit should " +
            "be used when sendiding measurements.",
            required = true, example = "1506697978", dataType = "long")
    public long time;

    public ChildLoginResponse(int id, long time) {
        this.id = id;
        this.time = time;
    }
}
