package nl.han.weatherboys.jorgapi;

import com.mashape.unirest.http.HttpResponse;
import com.mashape.unirest.http.Unirest;
import com.mashape.unirest.http.exceptions.UnirestException;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

@Service
class ResetTokenService {

    public void resetToken(String username, String password, String url) throws IOException {
        try {
            String requestToken = getLoginPage(url);
            postLogin(url, username, password, requestToken);
            postRestToken(url);
        } catch (UnirestException e) {
            throw new IOException(e);
        }
    }

    private void postLogin(String url, String username, String password, String requestToken) throws UnirestException {
        Unirest.post(url + "/Account/Login")
                .header("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8")
                .header("Origin", "http://iot.jorgvisch.nl")
                .header("Content-Type", "application/x-www-form-urlencoded")
                .header("Pragma", "no-cache")
                .header("Cache-Control", "no-cache")
                .header("Accept-Language", "nl-NL,nl;q=0.8,en-US;q=0.6,en;q=0.4")
                .field("__RequestVerificationToken", requestToken)
                .field("Email", username)
                .field("Password", password)
                .field("RememberMe", "false").asString();

    }

    private void postRestToken(String url) throws UnirestException {
        Unirest.post(url+"/Manage/ResetTokenRequest")
                .header("Content-Type", "application/x-www-form-urlencoded")
                .field("resettoken", "Reset token").asString();
    }

    private String getLoginPage(String url) throws UnirestException {
        HttpResponse<String> response = Unirest.get(url+"/Account/Login")
                .asString();

        Pattern pattern = Pattern.compile("value=\"([A-Za-z0-9_-]+)\"");
        Matcher matcher = pattern.matcher(response.getBody());
        matcher.find();
        return matcher.group(1);
    }

}
