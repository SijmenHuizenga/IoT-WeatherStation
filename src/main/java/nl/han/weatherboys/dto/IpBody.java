package nl.han.weatherboys.dto;

import io.swagger.annotations.ApiModelProperty;

public class IpBody {

    @ApiModelProperty(notes = "The local ip adress of the device.",
            required = true, example = "192.168.178.6", dataType = "string")
    public String ip;

    public IpBody(String ip) {
        this.ip = ip;
    }

    public IpBody() {
    }
}
