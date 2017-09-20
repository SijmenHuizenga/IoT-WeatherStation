package nl.han.weatherboys.ext;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.mashape.unirest.http.HttpResponse;
import com.mashape.unirest.http.JsonNode;
import com.mashape.unirest.http.Unirest;
import com.mashape.unirest.http.exceptions.UnirestException;
import nl.han.weatherboys.storage.model.Child;
import nl.han.weatherboys.storage.model.JorgApiCredential;
import org.json.JSONException;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;

@Service
public class JorgApi {

    private static final String URL_TOKEN = "/token";
    private static final String URL_DATA = "/api/Weather";

    private static final SimpleDateFormat TIMESTAMP_FORMAT = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.X");
    private static final SimpleDateFormat TIMESTAMP_FORMATOUT = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.0000000X");
    private static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss z");

    private final ObjectMapper mapper;

    @Autowired
    public JorgApi(ObjectMapper mapper){
        this.mapper = mapper;
    }

    public void tryUpdateToken(JorgApiCredential credentials) throws IOException {
        HttpResponse<JsonNode> response;
        try{
            response = Unirest.post(credentials.baseurl + URL_TOKEN)
                    .header("Content-Type", "application/x-www-form-urlencoded")
                    .header("Connection", "close")
                    .field("grant_type", "password")
                    .field("username", credentials.username)
                    .field("password", credentials.password)
                    .asJson();
        }catch (UnirestException e){
            throw new IOException(e);
        }
        handleErrorResponse(response);

        try{
            JSONObject body = response.getBody().getObject();
            String token = body.getString("access_token");
            Long expires = makeDateTime(body.getString(".expires"));

            if(expires == null)
                throw new IOException("No valid 'expires' field in response");
            if(token == null)
                throw new IOException("No token in response");
            credentials.token = token;
            credentials.tokenExpires = expires;

        }catch (JSONException e){
            throw new IOException(e);
        }
    }

    private void handleErrorResponse(HttpResponse<JsonNode> response) throws IOException {
        try {
            JSONObject body = response.getBody().getObject();
            if(response.getStatus() != 200) {
                String error = response.getStatus() + ":";
                if(body.has("error"))
                    error += " " + body.getString("error");
                else if(body.has(""))
                    error += " " + body.getString("error_description");
                System.out.println(body);
                throw new IOException(error);
            }
        } catch (JSONException e) {
            throw new IOException(e);
        }
    }

    public void postData(JorgApiCredential credentials, PostData data) throws IOException {
        HttpResponse<JsonNode> response;
        try{
            response = Unirest.post(credentials.baseurl + URL_DATA)
                    .header("Connection", "close")
                    .header("Content-Type", "application/json")
                    .header("Authorization", "bearer " + credentials.token)
                    .body(mapper.writeValueAsString(data))
                    .asJson();
        }catch (UnirestException e){
            throw new IOException(e);
        }

        handleErrorResponse(response);

        System.out.println(response);
    }

    public void postData(
            JorgApiCredential credentials, String weatherstation, long timestamp, float temperature, float illuminance)
            throws IOException {
        this.postData(credentials, new PostData(weatherstation, makeStringStamp(timestamp), temperature, illuminance));
    }

    class PostData {
        public String Weatherstation;
        public String Timestamp;
        public float Temperature;
        public float Illuminance;

        public PostData(String weatherstation, String timestamp, float temperature, float illuminance) {
            Weatherstation = weatherstation;
            Timestamp = timestamp;
            Temperature = temperature;
            Illuminance = illuminance;
        }
    }

    /**
     * Given a timestamp string in a response from the Jorg Api returns a a unix second timestemp. Or null if the format is invalid.
     * Example: 2017-09-19T09:29:44.1648619Z
     */
    Long makeTimestamp(String txt){
        try{
            Calendar c = Calendar.getInstance();
            //substring to remove the 7 digit fractal seconds and add back the last timezone char
            //
            //
            c.setTime(TIMESTAMP_FORMAT.parse(txt.substring(0, 20)+txt.charAt(27)));
            return c.getTimeInMillis()/1000L;
        } catch (ParseException e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * Given a datetime string in a response from the Jorg Api returns a unix second timestemp. Or null if the format is invalid.
     * Example: Wed, 20 Sep 2017 12:26:47 GMT
     */
    Long makeDateTime(String txt){
        try{
            Calendar c = Calendar.getInstance();
            c.setTime(DATE_FORMAT.parse(txt));
            return c.getTimeInMillis()/1000L;
        } catch (ParseException e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * Given a unix second timestamp return a timestamp in the following format:
     *    2017-09-20T15:53:47.0000000+02:00
     */
    String makeStringStamp(long timestamp){
        Calendar c = Calendar.getInstance();
        c.setTimeInMillis(timestamp*1000L);
        return TIMESTAMP_FORMATOUT.format(c.getTime());
    }

}
