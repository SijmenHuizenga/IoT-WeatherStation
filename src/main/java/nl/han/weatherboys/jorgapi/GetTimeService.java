package nl.han.weatherboys.jorgapi;

import com.mashape.unirest.http.HttpResponse;
import com.mashape.unirest.http.Unirest;
import com.mashape.unirest.http.exceptions.UnirestException;
import nl.han.weatherboys.storage.model.JorgApiCredential;
import org.hamcrest.text.MatchesPattern;
import org.json.JSONException;
import org.json.JSONObject;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.util.regex.Pattern;

import static org.hamcrest.MatcherAssert.assertThat;

@Service
public class GetTimeService extends JorgApiRequest {

    private static final String URL = "/api/time";

    /**
     * Get the time form the jorg api in Unix Timestmap Seconds
     */
    public long getTime(String baseurl, String token) throws IOException {
        HttpResponse<String> response;
        try{
            response = Unirest.get(baseurl + URL)
                    .header("Connection", "close")
                    .header("Content-Type", "application/json")
                    .header("Authorization", "bearer " + token)
                    .asString();
        }catch (UnirestException e){
            throw new IOException(e);
        }

        try {
            JSONObject jsonBody = new JSONObject(response.getBody());
            handleErrorResponse(response.getStatus(), jsonBody);
            throw new IOException("Received JSON, but no exception. This is not following convention.");
        } catch (JSONException e) {
            return parseResponse(response.getBody());
        }
    }

    public long parseResponse(String textBody) throws IOException {
        if(!new MatchesPattern(Pattern.compile("\"\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}.\\d{7}Z\"")).matches(textBody))
            throw new IOException("Wrong string format");

        return makeTimestamp(textBody.substring(1, textBody.length()-1));
    }

}
