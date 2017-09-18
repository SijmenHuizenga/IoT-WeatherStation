package nl.han.weatherboys.controller;

import nl.han.weatherboys.dto.PutHumidity;
import nl.han.weatherboys.storage.model.Humidity;
import nl.han.weatherboys.storage.model.Station;
import nl.han.weatherboys.storage.repo.HumidityRepo;
import nl.han.weatherboys.storage.repo.StationRepo;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.time.Instant;

@RestController("humidity")
public class HumidityController {

    private final HumidityRepo humidityRepo;
    private final StationRepo stationRepo;

    @Autowired
    public HumidityController(HumidityRepo humidityRepo, StationRepo stationRepo) {
        this.humidityRepo = humidityRepo;
        this.stationRepo = stationRepo;
    }

    @RequestMapping(method = RequestMethod.PUT)
    public ResponseEntity<String> index(@RequestBody PutHumidity data) {
        Station station = stationRepo.findOne(data.stationid);
        if(station == null)
            return ResponseEntity.badRequest().body("Station does not exit");
        humidityRepo.save(new Humidity(station, Instant.now().getEpochSecond(), data.humidity));
        return ResponseEntity.ok().body("ok");
    }


}
