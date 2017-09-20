package nl.han.weatherboys.storage.model;

import javax.persistence.Column;
import javax.persistence.Id;

public class JorgApiCredential {

    @Id
    @Column(nullable = false)
    public String username;

    @Column(nullable = false)
    public String password;

    @Column(nullable = false)
    public String baseurl;

    public String token;

    public Long tokenExpires;

    public JorgApiCredential() {
    }

    public JorgApiCredential(String username, String password, String baseurl, String token, Long tokenExpires) {
        this.username = username;
        this.password = password;
        this.baseurl = baseurl;
        this.token = token;
        this.tokenExpires = tokenExpires;
    }

    public JorgApiCredential(String username, String password, String baseurl) {
        this(username, password, baseurl, null, null);
    }

    @Override
    public String toString() {
        return "JorgApiCredential{" +
                "username='" + username + '\'' +
                ", password='" + password + '\'' +
                ", baseurl='" + baseurl + '\'' +
                ", token='" + token + '\'' +
                ", tokenExpires=" + tokenExpires +
                '}';
    }
}
