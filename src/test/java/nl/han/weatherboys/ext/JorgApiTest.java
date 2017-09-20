package nl.han.weatherboys.ext;

import com.fasterxml.jackson.databind.ObjectMapper;
import nl.han.weatherboys.storage.model.JorgApiCredential;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.junit4.SpringRunner;

@RunWith(SpringRunner.class)
public class JorgApiTest {

    private JorgApi api = new JorgApi(new ObjectMapper());

    private JorgApiCredential credentials
            = new JorgApiCredential("sijmenhuizenga@gmail.com", "3Qj6bQ7aQwB5*R8iDSj1", "http://iot.jorgvisch.nl", "7IwTtBvz-YRLWe03f2caRd-ZTieVAxySwjUFUU0AD7gIRyCsFINjwbnaA0LG1W_RZue2i0DdsxGGpzaD31FUJV2CKW_pXlb5NurF7noSE_g_1XhSv-wrxdUG0LnJDxBGMzsLz_kQ1X4oGMAaquj9NLcO9MEGF7S90IKtE6V-v043gOjePOLe5TYjNvDOrtsX0C6hpphj5ZTwYk4Daw8xmD6D0Jpn1ar45pLucZPFbwpb7j-sisPqesCH4-V9g2ElX21JEdzY_b-ThX4MWhK6YbsbKHZlTicd9epFWaDUMR7YwLxjLmr4O_zTkdeJtgCDT8JCa5GexUe8v3203wtrAxPWQrO4pYDZoNYjqF5eGOyd390qrnbp2GHoyzBJ1nCUuJrpKaozsET6fSADqFepqTaAZByJXUpWHpR_aKapkEsyYBb94KJZux4f1dq82xE-0eARib56G0XpWOhFunhFs5-CGUUDAt73eFlY5vH4ivAB6JSYmJeAVQFFNtvLHdme", 0L);

    @Test
    public void makeCalendar() throws Exception {
        Assert.assertEquals(1505813384L, (long) api.makeTimestamp("2017-09-19T09:29:44.1648619Z"));
    }

    @Test
    public void makeDateTime() throws Exception {
        Assert.assertEquals(1505910407L, (long) api.makeDateTime("Wed, 20 Sep 2017 12:26:47 GMT"));
    }

    @Test
    public void makeStringStamp() throws Exception {
        Assert.assertEquals("2017-09-19T11:29:44.0000000+02", api.makeStringStamp(1505813384L));
    }

    @Test
    public void tryUpdateToken() throws Exception {
        api.tryUpdateToken(credentials);
    }

    @Test
    public void postData() throws Exception {
        api.postData(credentials, "ATEST", System.currentTimeMillis()/1000L, 25.3f, 55f);
    }
}