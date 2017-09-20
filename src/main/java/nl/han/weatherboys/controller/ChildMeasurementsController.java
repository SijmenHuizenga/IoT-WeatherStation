package nl.han.weatherboys.controller;

import nl.han.weatherboys.dto.PutData;
import nl.han.weatherboys.storage.model.*;
import nl.han.weatherboys.storage.repo.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.time.Instant;

import static nl.han.weatherboys.controller.TimeController.now;
import static nl.han.weatherboys.dto.ErrorResponse.eberallert;

@RestController
public class ChildMeasurementsController {

    private final BrightnessRepo brightnessRepo;
    private final ChildRepo childRepo;
    private final HumidityRepo humidityRepo;
    private final PressureRepo pressureRepo;
    private final TemperatureRepo temperatureRepo;

    @Autowired
    public ChildMeasurementsController(BrightnessRepo brightnessRepo, ChildRepo childRepo,
                                       HumidityRepo humidityRepo, PressureRepo pressureRepo, TemperatureRepo temperatureRepo) {
        this.brightnessRepo = brightnessRepo;
        this.childRepo = childRepo;
        this.humidityRepo = humidityRepo;
        this.pressureRepo = pressureRepo;
        this.temperatureRepo = temperatureRepo;
    }

    @RequestMapping(method = RequestMethod.PUT, value = "/child/{id}/measurements", name = "Register measurements from child")
    public ResponseEntity index(
            @PathVariable(value="id") Integer id,
            @RequestBody PutData data) {
        Child child = childRepo.findOne(id);
        if(child == null)
            return eberallert();

        long now = now();

        if(data.brightness != null)
            brightnessRepo.save(new Brightness(child, now, data.brightness));
        if(data.humidity != null)
            humidityRepo.save(new Humidity(child, now, data.humidity));
        if(data.pressure != null)
            pressureRepo.save(new Pressure(child, now, data.pressure));
        if(data.temperature != null)
            temperatureRepo.save(new Temperature(child, now, data.temperature));

        return ResponseEntity.ok().body("ok");
    }

}
