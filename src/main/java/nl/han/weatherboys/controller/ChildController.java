package nl.han.weatherboys.controller;

import nl.han.weatherboys.storage.model.Child;
import nl.han.weatherboys.storage.repo.ChildRepo;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import static nl.han.weatherboys.dto.ErrorResponse.eberallert;

@RestController
public class ChildController {

    private final ChildRepo childRepo;

    public ChildController(ChildRepo childRepo) {
        this.childRepo = childRepo;
    }

    @RequestMapping(method = RequestMethod.GET, value = "/child/")
    public ResponseEntity<Iterable<Child>> getAllChildren() {
        return ResponseEntity.ok().body(childRepo.findAll());
    }

    @RequestMapping(method = RequestMethod.GET, value = "/child/{id}")
    public ResponseEntity getAllChildren(@PathVariable("id") String id) {
        Child child = childRepo.findOne(Integer.parseInt(id));
        if(child == null)
            return eberallert();

        return ResponseEntity.ok().body(child);
    }

}
