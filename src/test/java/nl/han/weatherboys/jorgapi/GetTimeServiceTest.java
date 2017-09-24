package nl.han.weatherboys.jorgapi;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.junit4.SpringRunner;

import static org.junit.Assert.assertEquals;

@RunWith(SpringRunner.class)
public class GetTimeServiceTest {

    @Test
    public void parseResponse() throws Exception {
        GetTimeService service = new GetTimeService();
        long l = service.parseResponse("\"2017-09-24T21:08:43.4695606Z\"");
        assertEquals(1506287323, l);
    }

}