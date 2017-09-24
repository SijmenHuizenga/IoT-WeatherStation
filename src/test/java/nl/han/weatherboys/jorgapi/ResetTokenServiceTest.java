package nl.han.weatherboys.jorgapi;

import org.junit.Test;

import java.io.IOException;

public class ResetTokenServiceTest {

    @Test
    public void resetToken() throws IOException {
        ResetTokenService service = new ResetTokenService();

        service.resetToken("autotest@example.com", "123abC*", "http://iot.jorgvisch.nl");
    }

}