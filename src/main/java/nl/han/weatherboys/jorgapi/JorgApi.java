package nl.han.weatherboys.jorgapi;

import nl.han.weatherboys.dto.JorgApiCredentialsSettings;
import nl.han.weatherboys.storage.model.JorgApiCredential;
import nl.han.weatherboys.storage.repo.JorgApiCredentialRepo;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.io.IOException;

@Service
public class JorgApi {

    private final Logger LOGGER = Logger.getLogger(this.getClass());

    private final GetTokenService updateTokenService;
    private final PutDataService putDataService;
    private final GetTimeService getTimeService;
    private final ResetTokenService resetTokenService;

    private JorgApiCredentialRepo credentialRepo;
    private JorgApiCredential credential;

    @Autowired
    public JorgApi(GetTokenService updateToken, PutDataService putDataService, GetTimeService getTimeService,
                   ResetTokenService resetTokenService, JorgApiCredentialRepo credentialRepo) {
        this.updateTokenService = updateToken;
        this.putDataService = putDataService;
        this.getTimeService = getTimeService;
        this.resetTokenService = resetTokenService;
        this.credentialRepo = credentialRepo;

        initCredentials();
    }

    private void initCredentials(){
        this.credential = credentialRepo.findOne(1);
        if(this.credential == null) {
            LOGGER.warn("The database must contain one api credentials with id 1 to send data to the jorgapi.");
            LOGGER.warn("Now, the JORGAPI is disabled!");
        }
    }

    /**
     * Get the server time of the jorg server in unix seconds or null if it could not be loaded.
     */
    public Long getTime() throws IOException {
        if(credential == null)
            return System.currentTimeMillis()/1000;
        return doRequestWithToken(
                () -> getTimeService.getTime(credential.baseurl, credential.token)
        );
    }

    public void putData(String weatherstation, long timestamp, float temperature, float illuminance) throws IOException {
        if(credential == null)
            return;
        doRequestWithToken(() -> {
            putDataService.postData(
                    credential.baseurl, credential.token, weatherstation, timestamp, temperature, illuminance
            );
            return null;
        });
    }

    private <R> R doRequestWithToken(TokenRequestAction<R> runnable) throws IOException {
        if(shouldRefreshToken(this.credential)) {
            try{
                updateToken(this.credential);
                credentialRepo.save(this.credential);
            }catch (IOException e){
                if(e.getMessage().contains("tokenrequest_overload"))
                    resetToken(this.credential);
            }
        }

        try {
            return runnable.doRequest();
        } catch (IOException e) {
            try {
                resetToken(this.credential);
                updateToken(this.credential);
                credentialRepo.save(this.credential);
                return runnable.doRequest();
            } catch (IOException e1) {
               throw new IOException("First try failed with error message " + e.getMessage() + ". Second try failed with the following error", e1);
            }
        }
    }

    public JorgApiCredentialsSettings getCredentialsSettings() {
        if(credential == null)
            return new JorgApiCredentialsSettings();
        return new JorgApiCredentialsSettings(credential.username, credential.password, credential.baseurl);
    }

    public boolean setCredentialsSettings(JorgApiCredentialsSettings data) {
        JorgApiCredential credentialNew = new JorgApiCredential(1, data.username, data.password, data.baseurl);
        try{
            resetToken(credentialNew);
            updateToken(credentialNew);
        }catch (Exception e){
            LOGGER.info("Invalid credentials", e);
            return false;
        }
        if(this.credential != null)
            credentialRepo.delete(this.credential);
        credentialRepo.save(credentialNew);
        this.credential = credentialNew;
        return true;
    }

    @FunctionalInterface
    interface TokenRequestAction<R> {
        R doRequest() throws IOException;
    }

    private void updateToken(JorgApiCredential credential) throws IOException {
        GetTokenService.Token token =
                updateTokenService.getToken(credential.baseurl, credential.username, credential.password);
        credential.token = token.token;
        credential.tokenExpires = token.tokenExpires;
    }

    private void resetToken(JorgApiCredential credential) throws IOException {
        resetTokenService.resetToken(credential.username, credential.password, credential.baseurl);
    }

    private boolean shouldRefreshToken(JorgApiCredential credential){
        return credential.token == null ||
                credential.token.equals("") ||
                //should refresh if the token is already expired or if the token expires in the next 60 seconds
                credential.tokenExpires < System.currentTimeMillis() / 1000L + 60;
    }

}
