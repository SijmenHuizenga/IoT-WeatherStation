package nl.han.weatherboys.controller;

import io.swagger.annotations.*;
import nl.han.weatherboys.dto.ErrorResponse;
import nl.han.weatherboys.dto.OkResponse;
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
@Api(tags = "Children")
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

    @ApiOperation(value = "Register measurements from child",
            notes = "This call is used by weather children to register new weather information. This information is stored " +
                    "in the gateway database and is accessable through the GET /child/ endpoints. All measurement fields are optional. " +
                    "If a field is not specified than the gateway assumes that this child has no sensor for that measurement type. " +
                    "If the temperature field and brightness field are available than the data is asynchroniously forwared " +
                    "to the Jorg Api.")
    @ApiResponses({
            @ApiResponse(code=200, message = "OK", response = OkResponse.class),
            @ApiResponse(code=400, message = "Bad Request", response = ErrorResponse.class),
    })
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

        LOGGER.info("Received data from child and stored data in db: " + data);

        if(data.temperature != null && data.brightness != null){
            long finalNow = now;
            new Thread(() -> {
                try {
                    jorgService.putData(child.name, finalNow, data.temperature, data.brightness);
                    LOGGER.info("Forwarded data from child to jorgapi: {" + child.name +", "+ finalNow +", "+
                            data.temperature +", "+ data.brightness + "}");
                } catch (IOException e) {
                    LOGGER.error("Could not put data forward to JorgApi", e);
                }
            }).run();
        }

        return ResponseEntity.ok().body(new OkResponse());
    }

}
