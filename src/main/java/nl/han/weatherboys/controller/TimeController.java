package nl.han.weatherboys.controller;

import org.springframework.web.bind.annotation.RestController;

import java.time.Instant;

@RestController
public class TimeController {



    public static long now(){
        return Instant.now().getEpochSecond();
    }

}
