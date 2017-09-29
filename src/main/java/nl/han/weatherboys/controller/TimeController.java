package nl.han.weatherboys.controller;

import io.swagger.annotations.*;
import nl.han.weatherboys.dto.NowResponse;
import nl.han.weatherboys.jorgapi.JorgApi;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.io.IOException;

@RestController
@Api(tags="Time")
public class TimeController {

    private final Logger LOGGER = Logger.getLogger(this.getClass());

    private JorgApi jorgService;

    @Autowired
    public TimeController(JorgApi jorgService) {
        this.jorgService = jorgService;
    }

    @ApiOperation(value = "Get the current server time",
            notes = "Retreive the global server time that is used by all connected devices. Time timestamp is a number" +
                    "in the Unix Timestamp seconds format. The timestamp is retreived from the Jorg Api on every request." +
                    " If the Jorg Api is offline than the gateway server timestamp will be returned.",
            response = NowResponse.class)
    @RequestMapping(method = RequestMethod.GET, value = "/time/now", name = "Get the server time in unix timestamp seconds")
    public NowResponse now(){
        try {
            return new NowResponse(jorgService.getTime());
        } catch (IOException e) {
            LOGGER.error("Could not get jorgApi time. Falling back to local time", e);
            return new NowResponse(System.currentTimeMillis()/1000L);
        }
    }

}
