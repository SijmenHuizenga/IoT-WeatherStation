package nl.han.weatherboys.controller;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.mashape.unirest.http.HttpResponse;
import com.mashape.unirest.http.ObjectMapper;
import com.mashape.unirest.http.Unirest;
import com.mashape.unirest.http.exceptions.UnirestException;
import io.swagger.annotations.Api;
import io.swagger.annotations.ApiOperation;
import io.swagger.annotations.ApiResponse;
import io.swagger.annotations.ApiResponses;
import nl.han.weatherboys.dto.ChildSettingsData;
import nl.han.weatherboys.dto.ErrorResponse;
import nl.han.weatherboys.dto.OkResponse;
import nl.han.weatherboys.jorgapi.JorgApi;
import nl.han.weatherboys.storage.model.Child;
import nl.han.weatherboys.storage.repo.ChildRepo;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import javax.validation.Valid;

import java.io.IOException;

import static nl.han.weatherboys.dto.ErrorResponse.emberallert;
import static nl.han.weatherboys.dto.ErrorResponse.error;

@RestController
@Api(tags = "Children")
public class ChildSettingsController {

    private final Logger LOGGER = Logger.getLogger(this.getClass());

    private final ChildRepo childRepo;

    @Autowired
    public ChildSettingsController(ChildRepo childRepo, com.fasterxml.jackson.databind.ObjectMapper objectMapper) {
        this.childRepo = childRepo;
        Unirest.setObjectMapper(new ObjectMapper() {

            public <T> T readValue(String value, Class<T> valueType) {
                try {
                    return objectMapper.readValue(value, valueType);
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }

            public String writeValue(Object value) {
                try {
                    return objectMapper.writeValueAsString(value);
                } catch (JsonProcessingException e) {
                    throw new RuntimeException(e);
                }
            }
        });
    }

    @ApiOperation(value = "Update child settings", notes = "Update the settings of a child with new data")
    @ApiResponses({
            @ApiResponse(code=200, message = "OK", response = OkResponse.class),
            @ApiResponse(code=504, message = "Child could not be reached", response = ErrorResponse.class),
            @ApiResponse(code=400, message = "Bad request", response = ErrorResponse.class),
    })
    @RequestMapping(method = RequestMethod.PUT, value = "/child/{childid}/settings")
    public ResponseEntity putChildSettings(
            @PathVariable("childid") String childid,
            @RequestBody @Valid ChildSettingsData data
    ) {
        Child child = childRepo.findOne(Integer.parseInt(childid));
        if(child == null)
            return emberallert();

        LOGGER.info("PUT settings to child " + child.id + "(" + child.ip + ") " + data);
        HttpResponse<String> result;
        try {
            result = doRequestToArduino(
                    () -> Unirest.put("http://" + child.ip + "/settings").body(data).asString()
            );
        } catch (UnirestException e) {
            LOGGER.info("PUT settings could not reach child. Removing ip from db", e);
            child.ip = null;
            childRepo.save(child);
            return ResponseEntity.status(HttpStatus.GATEWAY_TIMEOUT).body(new ErrorResponse("Could not reach child"));
        }

        LOGGER.info("PUT settings response " + result.getStatus() + " " + result.getBody());
        if(result.getStatus() == 200)
            return ResponseEntity.ok().body(new OkResponse());
        else
            return error(result.getStatusText());
    }

    @ApiOperation(value = "Retreive child settings", notes = "Get the settings from the child.")
    @ApiResponses({
            @ApiResponse(code=200, message = "OK", response = ChildSettingsData.class),
            @ApiResponse(code=504, message = "Child could not be reached", response = ErrorResponse.class),
            @ApiResponse(code=400, message = "Bad request", response = ErrorResponse.class),
    })
    @RequestMapping(method = RequestMethod.GET, value = "/child/{childid}/settings")
    public ResponseEntity getChildSettings(
            @PathVariable("childid") String childid
    ) {
        Child child = childRepo.findOne(Integer.parseInt(childid));
        if(child == null)
            return emberallert();

        LOGGER.info("GET settings from child " + child.id + "(" + child.ip + ") ");

        HttpResponse<ChildSettingsData> result;
        try {
            result = doRequestToArduino(
                    () -> Unirest.get("http://" + child.ip + "/settings").asObject(ChildSettingsData.class)
            );
        } catch (UnirestException e) {
            LOGGER.info("GET settings could not reach child. Removing ip from repo", e);
            child.ip = null;
            childRepo.save(child);
            return ResponseEntity.status(HttpStatus.GATEWAY_TIMEOUT).body(new ErrorResponse("Could not reach child"));
        }

        LOGGER.info("GET settings response " + result.getStatus() + " " + result.getBody());
        if(result.getStatus() == 200)
            return ResponseEntity.ok().body(result.getBody());
        else
            return error(result.getStatusText());
    }


    @ApiOperation(value = "Ping the child", notes = "See if the child is online and available to recevie settings calls.")
    @ApiResponses({
            @ApiResponse(code=200, message = "OK", response = OkResponse.class),
            @ApiResponse(code=504, message = "Child could not be reached", response = ErrorResponse.class),
    })
    @RequestMapping(method = RequestMethod.GET, value = "/child/{childid}/ping")
    public ResponseEntity pingChild(
            @PathVariable("childid") String childid
    ) {
        Child child = childRepo.findOne(Integer.parseInt(childid));
        if(child == null)
            return emberallert();

        LOGGER.info("GET ping to child " + child.id + "(" + child.ip + ") ");
        HttpResponse<String> result;
        try {
            result = doRequestToArduino(
                    () -> Unirest.get("http://" + child.ip + "/ping").asString()
            );
        } catch (UnirestException e) {
            LOGGER.info("GET ping could not reach child. Unregistering ip from child.", e);
            child.ip = null;
            childRepo.save(child);
            return ResponseEntity.status(HttpStatus.GATEWAY_TIMEOUT).body(new ErrorResponse("Could not reach child"));
        }

        LOGGER.info("GET ping response " + result.getStatus() + " " + result.getBody());
        return ResponseEntity.ok().body(new OkResponse());
    }


    private <T> HttpResponse<T> doRequestToArduino(ArduinoRequestAction<T> runnable) throws UnirestException {
        UnirestException e = null;
        for(int i = 0; i < 5; i++){
            try {
                return runnable.doRequest();
            } catch (UnirestException e1) {
                e = e1;
            }
            try {
                Thread.sleep(123);
            } catch (InterruptedException e1) {
                e1.printStackTrace();
            }
        }
        throw e;
    }

    @FunctionalInterface
    interface ArduinoRequestAction<T> {
        HttpResponse<T> doRequest() throws UnirestException;
    }


}
