package nl.han.weatherboys.controller;

import com.mashape.unirest.http.HttpResponse;
import com.mashape.unirest.http.Unirest;
import com.mashape.unirest.http.exceptions.UnirestException;
import io.swagger.annotations.Api;
import io.swagger.annotations.ApiOperation;
import io.swagger.annotations.ApiResponse;
import io.swagger.annotations.ApiResponses;
import nl.han.weatherboys.dto.ChildSettingsData;
import nl.han.weatherboys.dto.ErrorResponse;
import nl.han.weatherboys.dto.OkResponse;
import nl.han.weatherboys.storage.model.Child;
import nl.han.weatherboys.storage.repo.ChildRepo;
import org.apache.log4j.Logger;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import javax.validation.Valid;

import static nl.han.weatherboys.dto.ErrorResponse.emberallert;

@RestController
@Api(tags = "Children")
public class ChildSettingsController {

    private final Logger LOGGER = Logger.getLogger(this.getClass());

    private final ChildRepo childRepo;

    public ChildSettingsController(ChildRepo childRepo) {
        this.childRepo = childRepo;
    }

    @ApiOperation(value = "Update child settings", notes = "Update the settings of a child with new data")
    @ApiResponses({
            @ApiResponse(code=200, message = "OK", response = OkResponse.class),
            @ApiResponse(code=504, message = "Child could not be reached", response = ErrorResponse.class),
    })
    @RequestMapping(method = RequestMethod.PUT, value = "/child/{childid}/settings")
    public ResponseEntity getAllChildren(
            @PathVariable("childid") String childid,
            @RequestBody @Valid ChildSettingsData data
    ) {
        Child child = childRepo.findOne(Integer.parseInt(childid));
        if(child == null)
            return emberallert();

        HttpResponse<String> result;
        try {
            LOGGER.info("PUT settings to child " + child.id + "(" + child.ip + ") " + data);
            result = Unirest.put(child.ip + "/settings").body(data).asString();
        } catch (UnirestException e) {
            return ResponseEntity.status(HttpStatus.GATEWAY_TIMEOUT).body(new ErrorResponse("Could not reach child"));
        }

        LOGGER.info("PUT settings response " + result.getStatus() + " " + result.getBody());

        return ResponseEntity.ok().body(new OkResponse());
    }



}
