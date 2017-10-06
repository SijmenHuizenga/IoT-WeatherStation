package nl.han.weatherboys.controller;

import io.swagger.annotations.Api;
import io.swagger.annotations.ApiOperation;
import io.swagger.annotations.ApiResponse;
import io.swagger.annotations.ApiResponses;
import nl.han.weatherboys.dto.ChildLoginResponse;
import nl.han.weatherboys.dto.ErrorResponse;
import nl.han.weatherboys.storage.model.Child;
import nl.han.weatherboys.storage.repo.ChildRepo;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Collections;

import static nl.han.weatherboys.dto.ErrorResponse.emberallert;

@RestController
@Api(tags = "Children")
public class ChildDataController {

    private final ChildRepo childRepo;

    public ChildDataController(ChildRepo childRepo) {
        this.childRepo = childRepo;
    }

    @ApiOperation(value = "All children data", notes = "Get info of all children available in the gateway without measurement data.")
    @ApiResponses({
            @ApiResponse(code=200, message = "OK", response = Child.class, responseContainer = "List"),
    })
    @RequestMapping(method = RequestMethod.GET, value = "/child")
    @CrossOrigin
    public ResponseEntity<Iterable<Child>> getAllChildren() {
        Iterable<Child> all = childRepo.findAll();
        for (Child child : all) {
            child.brightnesses = Collections.emptySet();
            child.humidities = Collections.emptySet();
            child.pressures = Collections.emptySet();
            child.temperatures = Collections.emptySet();
        }

        return ResponseEntity.ok().body(all);
    }

    @ApiOperation(value = "single child data", notes = "Get the data from one child in the gateway by it's id including" +
            " all measurement data.")
    @ApiResponses({
            @ApiResponse(code=200, message = "OK", response = Child.class),
    })
    @RequestMapping(method = RequestMethod.GET, value = "/child/{id}")
    @CrossOrigin
    public ResponseEntity getAllChildren(@PathVariable("id") String id) {
        Child child = childRepo.findOne(Integer.parseInt(id));
        if(child == null)
            return emberallert();

        return ResponseEntity.ok().body(child);
    }

}
