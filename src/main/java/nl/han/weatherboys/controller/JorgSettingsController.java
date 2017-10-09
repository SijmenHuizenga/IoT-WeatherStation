package nl.han.weatherboys.controller;

import io.swagger.annotations.Api;
import io.swagger.annotations.ApiOperation;
import io.swagger.annotations.ApiResponse;
import io.swagger.annotations.ApiResponses;
import nl.han.weatherboys.dto.ErrorResponse;
import nl.han.weatherboys.dto.JorgApiCredentialsSettings;
import nl.han.weatherboys.dto.OkResponse;
import nl.han.weatherboys.jorgapi.JorgApi;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import javax.validation.Valid;

import static nl.han.weatherboys.dto.ErrorResponse.error;
import static org.springframework.http.ResponseEntity.ok;

@RestController
@Api(tags = "JorgApi")
public class JorgSettingsController {

    private JorgApi jorgApi;

    @Autowired
    public JorgSettingsController(JorgApi jorgApi) {
        this.jorgApi = jorgApi;
    }

    @ApiOperation(value = "Update jorgapi settings", notes = "Update the settings of a for the connection to the jorgapi")
    @ApiResponses({
            @ApiResponse(code=200, message = "OK", response = OkResponse.class),
            @ApiResponse(code=400, message = "Bad request", response = ErrorResponse.class),
    })
    @RequestMapping(method = RequestMethod.PUT, value = "/jorgapi")
    public ResponseEntity putJorgSettings(
            @RequestBody @Valid JorgApiCredentialsSettings data
    ) {
        if(jorgApi.setCredentialsSettings(data))
            return ok(new OkResponse());
        return error("Invalid credentials.");
    }

    @ApiOperation(value = "Get jorgapi settings", notes = "Get the current configuration for communication to the jorgapi")
    @ApiResponses({
            @ApiResponse(code=200, message = "OK", response = JorgApiCredentialsSettings.class),
    })
    @RequestMapping(method = RequestMethod.GET, value = "/jorgapi")
    public ResponseEntity getJorgSettings() {
        return ok(jorgApi.getCredentialsSettings());
    }

}
