package nl.han.weatherboys.jorgapi;

import com.mashape.unirest.http.HttpResponse;
import com.mashape.unirest.http.JsonNode;
import com.mashape.unirest.http.Unirest;
import com.mashape.unirest.http.exceptions.UnirestException;
import org.json.JSONException;
import org.json.JSONObject;
import org.springframework.stereotype.Service;

import java.io.IOException;

@Service
public class GetTokenService extends JorgApiRequest {

    private static final String URL = "/token";

    /**
     * Tries to get the token in the given credentials. If fails a IOException is thrown. If the
     * updating succeeds than the new token is returned.
     */
    public Token getToken(String baseurl, String username, String password) throws IOException {
        HttpResponse<JsonNode> response;
        try{
            response = Unirest.post(baseurl + URL)
                    .header("Content-Type", "application/x-www-form-urlencoded")
                    .header("Connection", "close")
                    .field("grant_type", "password")
                    .field("username", username)
                    .field("password", password)
                    .asJson();
        }catch (UnirestException e){
            throw new IOException(e);
        }
        JSONObject body = response.getBody().getObject();
        handleErrorResponse(response.getStatus(), body);

        return parseResponse(body);
    }

    private Token parseResponse(JSONObject body) throws IOException {
        try{
            String token = body.getString("access_token");
            Long expires = makeDateTime(body.getString(".expires"));

            if(expires == null)
                throw new IOException("No valid 'expires' field in response");
            if(token == null)
                throw new IOException("No token in response");
            return new Token(token, expires);
        }catch (JSONException e){
            throw new IOException(e);
        }
    }

    class Token {
        String token;
        Long tokenExpires;

        public Token(String token, Long tokenExpires) {
            this.token = token;
            this.tokenExpires = tokenExpires;
        }

        @Override
        public String toString() {
            return "Token{" +
                    "token='" + token + '\'' +
                    ", tokenExpires=" + tokenExpires +
                    '}';
        }
    }

}
