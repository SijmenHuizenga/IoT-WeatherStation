package nl.han.weatherboys.dto;

import org.springframework.http.ResponseEntity;

public class ErrorResponse {

    public String message;

    public ErrorResponse(String message) {
        this.message = message;
    }

    public static ErrorResponse error(String message){
        return new ErrorResponse(message);
    }

    public static ResponseEntity<ErrorResponse> emberallert(){
        return ResponseEntity.badRequest().body(new ErrorResponse("EMBER ALLERT! Child not found!"));
    }
}
