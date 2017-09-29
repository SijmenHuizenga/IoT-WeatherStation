package nl.han.weatherboys.controller;

import nl.han.weatherboys.dto.ChildLoginResponse;
import nl.han.weatherboys.dto.IpBody;
import nl.han.weatherboys.jorgapi.JorgApi;
import nl.han.weatherboys.storage.model.Child;
import nl.han.weatherboys.storage.repo.ChildRepo;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;

import static nl.han.weatherboys.dto.ErrorResponse.emberallert;
import static nl.han.weatherboys.dto.ErrorResponse.error;

@RestController
public class ChildController {

    private final ChildRepo childRepo;
    private final JorgApi jorgApi;

    public ChildController(ChildRepo childRepo, JorgApi jorgApi) {
        this.childRepo = childRepo;
        this.jorgApi = jorgApi;
    }

    @RequestMapping(method = RequestMethod.GET, value = "/child/")
    public ResponseEntity<Iterable<Child>> getAllChildren() {
        return ResponseEntity.ok().body(childRepo.findAll());
    }

    @RequestMapping(method = RequestMethod.GET, value = "/child/{id}")
    public ResponseEntity getAllChildren(@PathVariable("id") String id) {
        Child child = childRepo.findOne(Integer.parseInt(id));
        if(child == null)
            return emberallert();

        return ResponseEntity.ok().body(child);
    }

    @RequestMapping(method = RequestMethod.POST, value = "/child/{id}/login")
    public ResponseEntity signInExistingChild(@PathVariable("id") String id, @RequestBody IpBody ip) {
        Child child = childRepo.findOne(Integer.parseInt(id));
        if(child == null)
            return emberallert();

        if(ip == null || ip.ip == null || ip.ip.isEmpty())
            return error("Body in the wrong format");

        child.ip = ip.ip;

        long time;
        try {
             time = jorgApi.getTime();
        } catch (IOException e) {
            e.printStackTrace();
            return error("Could not retrieve time");
        }

        return ResponseEntity.ok().body(
                new ChildLoginResponse(child.id, time)
        );
    }

    @RequestMapping(method = RequestMethod.POST, value = "/child/login")
    public ResponseEntity signInNewChild(@RequestBody IpBody ip) {
        if(ip == null || ip.ip == null || ip.ip.isEmpty())
            return error("Body in the wrong format");

        Child child = new Child("TMP");
        child.ip = ip.ip;

        childRepo.save(child);

        long time;
        try {
            time = jorgApi.getTime();
        } catch (IOException e) {
            e.printStackTrace();
            return error("Could not retrieve time");
        }

        return ResponseEntity.ok().body(
                new ChildLoginResponse(child.id, time)
        );
    }

}
