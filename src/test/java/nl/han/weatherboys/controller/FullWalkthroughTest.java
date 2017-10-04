package nl.han.weatherboys.controller;

import com.fasterxml.jackson.databind.ObjectMapper;
import nl.han.weatherboys.Applications;
import nl.han.weatherboys.JpaConfig;
import nl.han.weatherboys.dto.IpBody;
import nl.han.weatherboys.dto.PutData;
import nl.han.weatherboys.storage.model.JorgApiCredential;
import nl.han.weatherboys.storage.repo.JorgApiCredentialRepo;
import org.hamcrest.BaseMatcher;
import org.hamcrest.Description;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.annotation.Rollback;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringRunner;
import org.springframework.test.context.web.AnnotationConfigWebContextLoader;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.ResultActions;
import org.springframework.web.context.WebApplicationContext;

import javax.annotation.Resource;

import java.util.function.IntConsumer;

import static nl.han.weatherboys.TestGlobals.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;
import static org.springframework.test.web.servlet.setup.MockMvcBuilders.webAppContextSetup;

@RunWith(SpringRunner.class)
@SpringBootTest(classes = Applications.class)
@WebAppConfiguration
@ContextConfiguration(
        classes = {JpaConfig.class},
        loader = AnnotationConfigWebContextLoader.class)
public class FullWalkthroughTest {

    private MockMvc mockMvc;
    private ObjectMapper mapper = new ObjectMapper();

    @Resource
    private JorgApiCredentialRepo credentialRepo;

    @Autowired
    private WebApplicationContext webApplicationContext;

    @Before
    public void setup() throws Exception {
        this.mockMvc = webAppContextSetup(webApplicationContext).build();
        credentialRepo.save(new JorgApiCredential(1, JORGUSER, JORGPASS, JORGURL));
    }

    /**
     * This tests runs all api requests just like they would be used by a arduino child.
     * 1. Child register
     * 2. Put data (4x)
     * 3. Get all child data
     * 4. Login child with different ip
     * 5. Get single child data
     * 6. Get current time
     */
    @Test
    public void testEverything() throws Exception {

        int childid = registerchild();

        testPutData(childid, 50L, 50f, 30f, 23f, 20f).andExpect(status().isOk());
        testPutData(childid, 60L, 50.0f, null, 23.9f, null).andExpect(status().isOk());
        testPutData(childid, 70L, null, 30.5f, null, 23.3f).andExpect(status().isOk());
        testPutData(childid, 80L, null, null, null, null).andExpect(status().isOk());

        mockMvc.perform(
                get("/child")
                        .accept(MediaType.APPLICATION_JSON))
                .andDo(mvcResult -> System.out.println(mvcResult.getResponse().getContentAsString()))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$[0].id").value(childid))
                .andExpect(jsonPath("$[0].ip").value("192.168.178.0.1"))

                .andExpect(jsonPath("$[0].brightnesses[0].moment").value(50L))
                .andExpect(jsonPath("$[0].brightnesses[0].brightness").value(30f))
                .andExpect(jsonPath("$[0].brightnesses[1].moment").value(70L))
                .andExpect(jsonPath("$[0].brightnesses[1].brightness").value(30.5f))

                .andExpect(jsonPath("$[0].humidities[0].moment").value(50f))
                .andExpect(jsonPath("$[0].humidities[0].humidity").value(50.0f))
                .andExpect(jsonPath("$[0].humidities[1].moment").value(60f))
                .andExpect(jsonPath("$[0].humidities[1].humidity").value(50.0f))

                .andExpect(jsonPath("$[0].pressures[0].pressure").value(20f))
                .andExpect(jsonPath("$[0].pressures[1].pressure").value(23.3f))

                .andExpect(jsonPath("$[0].temperatures[0].temperature").value(23f))
                .andExpect(jsonPath("$[0].temperatures[1].temperature").value(23.9f));

        mockMvc.perform(
                post("/child/" + childid + "/login")
                        .content(mapper.writeValueAsString(new IpBody("192.168.178.0.5")))
                        .contentType(MediaType.APPLICATION_JSON)
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").value(childid));

        mockMvc.perform(
                get("/child/" + childid)
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").value(childid))
                .andExpect(jsonPath("$.ip").value("192.168.178.0.5"));

        mockMvc.perform(
                get("/time/now")
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.time").exists());

    }

    @Test
    public void testBadMeasurementData() throws Exception {
        int childid = registerchild();

        long realtime = System.currentTimeMillis()/1000L;

        //null time is not allowed
        testPutData(childid, null, null, null, null, null).andExpect(status().isBadRequest());
        testPutData(childid, -1L, null, null, null, null).andExpect(status().isBadRequest());

        //invalid child id
        testPutData(-1, realtime, null, null, null, null).andExpect(status().isBadRequest());

        //Humidity must be between 0 and 80
        testPutData(childid, realtime, 80.5f, null, null, null).andExpect(status().isBadRequest());
        testPutData(childid, realtime, 81f, null, null, null).andExpect(status().isBadRequest());
        testPutData(childid, realtime, -1f, null, null, null).andExpect(status().isBadRequest());

        //brightness must be between 0 and 100
        testPutData(childid, realtime, null, 100.5f, null, null).andExpect(status().isBadRequest());
        testPutData(childid, realtime, null, 101f, null, null).andExpect(status().isBadRequest());
        testPutData(childid, realtime, null, -1f, null, null).andExpect(status().isBadRequest());

        //temperature must be between 10 and 85
        testPutData(childid, realtime, null, null, 9.9f, null).andExpect(status().isBadRequest());
        testPutData(childid, realtime, null, null, 85.3f, null).andExpect(status().isBadRequest());
        testPutData(childid, realtime, null, null, 86.1f, null).andExpect(status().isBadRequest());

        //pressure must be between 0 and ???
        testPutData(childid, realtime, null, null, null, -1f).andExpect(status().isBadRequest());
    }

    private int registerchild() throws Exception {
        final int[] childid = {-1};

        mockMvc.perform(
                post("/child/register")
                        .content(mapper.writeValueAsString(new IpBody("192.168.178.0.1")))
                        .contentType(MediaType.APPLICATION_JSON)
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andDo(mvcResult -> System.out.println(mvcResult.getResponse().getContentAsString()))
                .andExpect(jsonPath("$.id").exists())
                .andDo(mvcResult -> System.out.println(mvcResult.getResponse().getContentAsString()))
                .andExpect(jsonPath("$.id").value(getIdExtractor(id -> childid[0] = id)));
        return childid[0];
    }

    private ResultActions testPutData(int childid, Long timestamp, Float humidity, Float brightness, Float temperature, Float pressure) throws Exception {
        return mockMvc.perform(
                put("/child/" + childid + "/measurements/")
                        .content(mapper.writeValueAsString(new PutData(timestamp, humidity, brightness, temperature, pressure)))
                        .contentType(MediaType.APPLICATION_JSON)
                        .accept(MediaType.APPLICATION_JSON));
    }

    private BaseMatcher<Integer> getIdExtractor(IntConsumer value) {
        return new BaseMatcher<Integer>() {
            @Override
            public void describeTo(Description description) {
            }

            @Override
            public boolean matches(Object item) {
                value.accept((Integer) item);
                return true;
            }
        };
    }
}