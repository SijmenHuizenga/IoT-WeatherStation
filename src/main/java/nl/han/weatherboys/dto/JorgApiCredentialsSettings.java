package nl.han.weatherboys.dto;

import javax.persistence.Column;
import javax.validation.constraints.NotNull;

public class JorgApiCredentialsSettings {

    @NotNull
    public String username;

    @NotNull
    public String password;

    @NotNull
    public String baseurl;

    public JorgApiCredentialsSettings(String username, String password, String baseurl) {
        this.username = username;
        this.password = password;
        this.baseurl = baseurl;
    }

    public JorgApiCredentialsSettings() {
    }
}
