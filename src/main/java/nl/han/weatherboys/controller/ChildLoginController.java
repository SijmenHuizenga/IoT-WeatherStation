package nl.han.weatherboys.controller;

import io.swagger.annotations.Api;
import io.swagger.annotations.ApiOperation;
import io.swagger.annotations.ApiResponse;
import io.swagger.annotations.ApiResponses;
import nl.han.weatherboys.dto.ChildLoginResponse;
import nl.han.weatherboys.dto.ErrorResponse;
import nl.han.weatherboys.dto.IpBody;
import nl.han.weatherboys.dto.OkResponse;
import nl.han.weatherboys.jorgapi.JorgApi;
import nl.han.weatherboys.storage.model.Child;
import nl.han.weatherboys.storage.repo.ChildRepo;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;

import static nl.han.weatherboys.dto.ErrorResponse.emberallert;
import static nl.han.weatherboys.dto.ErrorResponse.error;

@RestController
@Api(tags = "Children")
public class ChildLoginController {

    private final ChildRepo childRepo;
    private final JorgApi jorgApi;

    public ChildLoginController(ChildRepo childRepo, JorgApi jorgApi) {
        this.childRepo = childRepo;
        this.jorgApi = jorgApi;
    }

    @ApiOperation(value = "Login an existing child",
            notes = "When an existing child starts up and comes online, it will use this call to login on the gateway. " +
                    "With this login request the child sends its local ip address so the gateway can reach the child when needed. " +
                    "The response also contains the current server timestamp as would be returned by GET /time/now. This is a " +
                    "conveinience for the child so it does not need to fire another request to the gateway for time syncing.")
    @ApiResponses({
            @ApiResponse(code=200, message = "OK", response = ChildLoginResponse.class),
            @ApiResponse(code=400, message = "Bad Request", response = ErrorResponse.class),
    })
    @RequestMapping(method = RequestMethod.POST, value = "/child/{id}/login")
    public ResponseEntity signInExistingChild(
            @PathVariable("id") String id,
            @RequestBody IpBody ip) {
        Child child = childRepo.findOne(Integer.parseInt(id));
        if(child == null)
            return emberallert();

        if(ip == null || ip.ip == null || ip.ip.isEmpty())
            return error("Body in the wrong format");

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


    @ApiOperation(value = "Register a new child",
            notes = "When a child comes online for the first time, the child will use this endpoint to register ittsself " +
                    "and receive a brand new childid." +
                    "With this login request the child sends its local ip address so the gateway can reach the child when needed. " +
                    "The response also contains the current server timestamp as would be returned by GET /time/now. This is a " +
                    "conveinience for the child so it does not need to fire another request to the gateway for time syncing.")
    @ApiResponses({
            @ApiResponse(code=200, message = "OK", response = ChildLoginResponse.class),
            @ApiResponse(code=400, message = "Bad Request", response = ErrorResponse.class),
    })
    @RequestMapping(method = RequestMethod.POST, value = "/child/register")
    public ResponseEntity signInNewChild(@RequestBody IpBody ip) {
        if(ip == null || ip.ip == null || ip.ip.isEmpty())
            return error("Body in the wrong format");

        Child child = new Child("NEW");
        child.ip = ip.ip;

        childRepo.save(child);
        child.name = "NEW" + child.id;
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
