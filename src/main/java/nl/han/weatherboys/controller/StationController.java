package nl.han.weatherboys.controller;

import nl.han.weatherboys.storage.model.Station;
import nl.han.weatherboys.storage.repo.StationRepo;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

@RestController("station")
public class StationController {

    private final StationRepo stationRepo;

    @Autowired
    public StationController(StationRepo stationRepo) {
        this.stationRepo = stationRepo;
    }

    @RequestMapping(method = RequestMethod.GET)
    public ResponseEntity<Iterable<Station>> getAll() {
        Iterable<Station> all = stationRepo.findAll();

        return ResponseEntity.ok().body(all);
    }

}
