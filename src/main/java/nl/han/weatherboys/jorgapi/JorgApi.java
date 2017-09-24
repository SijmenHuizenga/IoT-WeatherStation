package nl.han.weatherboys.jorgapi;

import nl.han.weatherboys.storage.model.JorgApiCredential;
import nl.han.weatherboys.storage.repo.JorgApiCredentialRepo;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.io.IOException;

@Service
public class JorgApi {

    private final GetTokenService updateTokenService;
    private final PutDataService putDataService;
    private final GetTimeService getTimeService;

    private JorgApiCredentialRepo credentialRepo;
    private JorgApiCredential credential;

    @Autowired
    public JorgApi(GetTokenService updateToken, PutDataService putDataService, GetTimeService getTimeService,
                   JorgApiCredentialRepo credentialRepo) {
        this.updateTokenService = updateToken;
        this.putDataService = putDataService;
        this.getTimeService = getTimeService;
        this.credentialRepo = credentialRepo;
    }

    private void initCredentials(){
        if(this.credential != null)
            return;
        this.credential = credentialRepo.findOne(1);
        if(this.credential == null)
            throw new IllegalStateException("The database must contain one api credentials with id 1");
    }

    /**
     * Get the server time of the jorg server in unix seconds or null if it could not be loaded.
     */
    public Long getTime(){
        return doRequestWithToken(
                () -> getTimeService.getTime(credential.baseurl, credential.token)
        );
    }

    public void putData(String weatherstation, long timestamp, float temperature, float illuminance){
        doRequestWithToken(() -> {
            putDataService.postData(
                    credential.baseurl, credential.token, weatherstation, timestamp, temperature, illuminance
            );
            return null;
        });
    }

    protected <R> R doRequestWithToken(TokenRequestAction<R> runnable){
        initCredentials();
        if(shouldRefreshToken(this.credential))
            try {
                updateToken();
            } catch (IOException e) {
                e.printStackTrace();
                return null;
            }
        try {
            return runnable.doRequest();
        } catch (IOException e) {
            try {
                updateToken();
                return runnable.doRequest();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
        return null;
    }

    @FunctionalInterface
    interface TokenRequestAction<R> {
        R doRequest() throws IOException;
    }

    private void updateToken() throws IOException {
        GetTokenService.Token token =
                updateTokenService.getToken(credential.baseurl, credential.username, credential.password);
        credential.token = token.token;
        credential.tokenExpires = token.tokenExpires;
        credentialRepo.save(credential);
    }

    private boolean shouldRefreshToken(JorgApiCredential credential){
        return credential.token == null ||
                credential.token.equals("") ||
                //should refresh if the token is already expired or if the token expires in the next 60 seconds
                credential.tokenExpires < System.currentTimeMillis() / 1000L + 60;
    }

}
