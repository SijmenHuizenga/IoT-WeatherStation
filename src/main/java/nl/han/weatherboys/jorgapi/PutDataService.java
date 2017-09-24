package nl.han.weatherboys.jorgapi;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.mashape.unirest.http.HttpResponse;
import com.mashape.unirest.http.JsonNode;
import com.mashape.unirest.http.Unirest;
import com.mashape.unirest.http.exceptions.UnirestException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.io.IOException;

@Service
public class PutDataService extends JorgApiRequest {

    private static final String URL = "/api/Weather";

    private final ObjectMapper mapper;

    @Autowired
    public PutDataService(ObjectMapper mapper){
        this.mapper = mapper;
    }

    private void postData(String baseurl, String token, PostData data) throws IOException {
        HttpResponse<JsonNode> response;
        try{
            response = Unirest.post(baseurl + URL)
                    .header("Connection", "close")
                    .header("Content-Type", "application/json")
                    .header("Authorization", "bearer " + token)
                    .body(mapper.writeValueAsString(data))
                    .asJson();
        }catch (UnirestException e){
            throw new IOException(e);
        }

        handleErrorResponse(response.getStatus(), response.getBody().getObject());
    }

    /**
     * Posts the given data to the Jorg Api with the given credentials. If a error is received than a IOException
     * is thrown. If the data is successfully sent than no Exception is thrown.
     * All fields are required!
     */
    public void postData(
            String baseurl, String token, String weatherstation, long timestamp, float temperature, float illuminance)
            throws IOException {
        this.postData(baseurl, token, new PostData(weatherstation, makeStringStamp(timestamp), temperature, illuminance));
    }

    class PostData {
        public String Weatherstation;
        public String Timestamp;
        public float Temperature;
        public float Illuminance;

        /**
         * All fields are required!
         */
        public PostData(String weatherstation, String timestamp, float temperature, float illuminance) {
            Weatherstation = weatherstation;
            Timestamp = timestamp;
            Temperature = temperature;
            Illuminance = illuminance;
        }
    }

}
