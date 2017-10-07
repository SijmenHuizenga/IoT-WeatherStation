package nl.han.weatherboys.jorgapi;

import org.json.JSONObject;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mockito;
import org.springframework.test.context.junit4.SpringRunner;

import java.io.IOException;

@RunWith(SpringRunner.class)
public class JorgApiRequestTest {

    private JorgApiRequest my = Mockito.mock(JorgApiRequest.class, Mockito.CALLS_REAL_METHODS);

    @Test
    public void makeTimestamp() throws Exception {
        Assert.assertEquals(1505813384L, (long) my.makeTimestamp("2017-09-19T09:29:44.1648619Z"));
    }

    @Test
    public void makeDateTime() throws Exception {
        Assert.assertEquals(1505910407L, (long) my.makeDateTime("Wed, 20 Sep 2017 12:26:47 GMT"));
    }

    @Test
    public void makeTimestampWrong() throws Exception {
        Assert.assertNull(my.makeTimestamp("wrongformat"));
    }

    @Test
    public void makeDateTimeWrong() throws Exception {
        Assert.assertNull(my.makeDateTime("wrongformat"));
    }

    @Test
    public void handleErrorResponseOk() throws Exception {
        my.handleErrorResponse(200, new JSONObject("{}"));
    }

    @Test(expected = IOException.class)
    public void handleErrorResponseNormalError() throws Exception {
        my.handleErrorResponse(400, new JSONObject("{\"error\": \"Test Error\", \"error_description\": \"adsf\"}"));
    }

    @Test(expected = IOException.class)
    public void handleErrorResponseError() throws Exception {
        my.handleErrorResponse(400, new JSONObject("{\"error\": \"Test Error\", \"error_description\": null}"));
    }


}