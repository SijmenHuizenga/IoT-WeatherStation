package nl.han.weatherboys.controller;

import nl.han.weatherboys.dto.NowResponse;
import nl.han.weatherboys.jorgapi.JorgApi;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.io.IOException;
import java.time.Instant;

@RestController
public class TimeController {

    private final Logger LOGGER = Logger.getLogger(this.getClass());

    private final JorgApi jorgService;

    @Autowired
    public TimeController(JorgApi jorgService) {
        this.jorgService = jorgService;
    }

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
