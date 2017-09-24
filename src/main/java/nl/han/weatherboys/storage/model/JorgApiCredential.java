package nl.han.weatherboys.storage.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;

@Entity
public class JorgApiCredential {

    @Id
    public int id;

    @Column(nullable = false)
    public String username;

    @Column(nullable = false)
    public String password;

    @Column(nullable = false)
    public String baseurl;

    @Column(length = 800)
    public String token;

    public Long tokenExpires;

    public JorgApiCredential() {
    }

    public JorgApiCredential(int id, String username, String password, String baseurl, String token, Long tokenExpires) {
        this.id = id;
        this.username = username;
        this.password = password;
        this.baseurl = baseurl;
        this.token = token;
        this.tokenExpires = tokenExpires;
    }

    public JorgApiCredential(int id, String username, String password, String baseurl) {
        this(id, username, password, baseurl, null, null);
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
