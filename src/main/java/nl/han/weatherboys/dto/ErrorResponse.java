package nl.han.weatherboys.dto;

import io.swagger.annotations.ApiModelProperty;
import org.springframework.http.ResponseEntity;

import javax.validation.constraints.NotNull;

public class ErrorResponse {

    @ApiModelProperty(notes = "A string describing the error that occeured.", required = true)
    public String message;

    public ErrorResponse(@NotNull String message) {
        this.message = message;
    }

    public static ResponseEntity<ErrorResponse> error(@NotNull String message){
        return ResponseEntity.badRequest().body(new ErrorResponse(message));
    }

    public static ResponseEntity<ErrorResponse> emberallert(){
        return ResponseEntity.badRequest().body(new ErrorResponse("EMBER ALLERT! Child not found!"));
    }
}
