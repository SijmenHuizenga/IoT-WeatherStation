package nl.han.weatherboys.controller;

import nl.han.weatherboys.dto.PutData;
import nl.han.weatherboys.jorgapi.JorgApi;
import nl.han.weatherboys.storage.model.*;
import nl.han.weatherboys.storage.repo.*;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;

import static nl.han.weatherboys.dto.ErrorResponse.emberallert;

@RestController
public class ChildMeasurementsController {

    private final Logger LOGGER = Logger.getLogger(this.getClass());

    private final BrightnessRepo brightnessRepo;
    private final ChildRepo childRepo;
    private final HumidityRepo humidityRepo;
    private final PressureRepo pressureRepo;
    private final TemperatureRepo temperatureRepo;

    private final JorgApi jorgService;

    @Autowired
    public ChildMeasurementsController(BrightnessRepo brightnessRepo, ChildRepo childRepo,
                                       HumidityRepo humidityRepo, PressureRepo pressureRepo,
                                       TemperatureRepo temperatureRepo, JorgApi jorgService) {
        this.brightnessRepo = brightnessRepo;
        this.childRepo = childRepo;
        this.humidityRepo = humidityRepo;
        this.pressureRepo = pressureRepo;
        this.temperatureRepo = temperatureRepo;
        this.jorgService = jorgService;
    }

    /**
     * @param childid Weather Child Station ID
     * @param data Measurement Data
     */
    @RequestMapping(method = RequestMethod.PUT, value = "/child/{childid}/measurements", name = "Register measurements from child")
    public ResponseEntity registerMeasurement(
            @PathVariable(value="childid") Integer childid,
            @RequestBody PutData data) {
        Child child = childRepo.findOne(childid);
        if(child == null)
            return emberallert();

        long now;
        try {
            now = jorgService.getTime();
        } catch (IOException e) {
            LOGGER.error("Could not get jorgApi time. Falling back to local time", e);
            now = System.currentTimeMillis()/1000L;
        }

        if(data.brightness != null)
            brightnessRepo.save(new Brightness(child, now, data.brightness));
        if(data.humidity != null)
            humidityRepo.save(new Humidity(child, now, data.humidity));
        if(data.pressure != null)
            pressureRepo.save(new Pressure(child, now, data.pressure));
        if(data.temperature != null)
            temperatureRepo.save(new Temperature(child, now, data.temperature));

        if(data.temperature != null && data.brightness != null)
            try {
                jorgService.putData(child.name, now, data.temperature, data.brightness);
            } catch (IOException e) {
                LOGGER.error("Could not put data forward to JorgApi", e);
            }

        return ResponseEntity.ok().body("ok");
    }

}
