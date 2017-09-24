package nl.han.weatherboys.jorgapi;

import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mockito;
import org.springframework.test.context.junit4.SpringRunner;

import static org.junit.Assert.*;

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
    public void makeStringStamp() throws Exception {
        Assert.assertEquals("2017-09-19T11:29:44.0000000+02", my.makeStringStamp(1505813384L));
    }

}