package nl.han.weatherboys.jorgapi;

import org.junit.Test;
import java.io.IOException;

import static nl.han.weatherboys.TestGlobals.*;

public class ResetTokenServiceTest {

    @Test
    public void resetToken() throws IOException {
        ResetTokenService service = new ResetTokenService();

        service.resetToken(JORGUSER, JORGPASS, JORGURL);
    }

}