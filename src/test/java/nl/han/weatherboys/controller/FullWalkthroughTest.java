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
    @Rollback
    public void testEverything() throws Exception {
        final int[] childid = {-1};

        mockMvc.perform(
                post("/child/register")
                        .content(mapper.writeValueAsString(new IpBody("192.168.178.0.1")))
                        .contentType(MediaType.APPLICATION_JSON)
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").exists())
                .andExpect(jsonPath("$.id").value(new BaseMatcher<Integer>() {
                    @Override
                    public void describeTo(Description description) {
                    }

                    @Override
                    public boolean matches(Object item) {
                        childid[0] = (int) item;
                        return true;
                    }
                }));

        testPutData(childid[0], 50f, 30f, 23f, 20f).andExpect(status().isOk());
        testPutData(childid[0], 50.0f, null, 23.9f, null).andExpect(status().isOk());
        testPutData(childid[0], null, 30.5f, null, 23.3f).andExpect(status().isOk());
        testPutData(childid[0], null, null, null, null).andExpect(status().isOk());

        mockMvc.perform(
                get("/child")
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$[0].id").value(childid[0]))
                .andExpect(jsonPath("$[0].ip").value("192.168.178.0.1"))
                .andExpect(jsonPath("$[0].brightnesses[0].brightness").value(30f))
                .andExpect(jsonPath("$[0].brightnesses[1].brightness").value(30.5f))
                .andExpect(jsonPath("$[0].humidities[0].humidity").value(50.0f))
                .andExpect(jsonPath("$[0].humidities[1].humidity").value(50.0f))
                .andExpect(jsonPath("$[0].pressures[0].pressure").value(20f))
                .andExpect(jsonPath("$[0].pressures[1].pressure").value(23.3f))
                .andExpect(jsonPath("$[0].temperatures[0].temperature").value(23f))
                .andExpect(jsonPath("$[0].temperatures[1].temperature").value(23.9f));

        mockMvc.perform(
                post("/child/" + childid[0] + "/login")
                        .content(mapper.writeValueAsString(new IpBody("192.168.178.0.5")))
                        .contentType(MediaType.APPLICATION_JSON)
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").value(childid[0]));

        mockMvc.perform(
                get("/child/" + childid[0])
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").value(childid[0]))
                .andExpect(jsonPath("$.ip").value("192.168.178.0.5"))
                .andExpect(jsonPath("$.brightnesses[0].brightness").value(30f))
                .andExpect(jsonPath("$.brightnesses[1].brightness").value(30.5f))
                .andExpect(jsonPath("$.humidities[0].humidity").value(50.0f))
                .andExpect(jsonPath("$.humidities[1].humidity").value(50.0f))
                .andExpect(jsonPath("$.pressures[0].pressure").value(20f))
                .andExpect(jsonPath("$.pressures[1].pressure").value(23.3f))
                .andExpect(jsonPath("$.temperatures[0].temperature").value(23f))
                .andExpect(jsonPath("$.temperatures[1].temperature").value(23.9f));

        mockMvc.perform(
                get("/time/now")
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.time").exists());

    }

    private ResultActions testPutData(int childid, Float humidity, Float brightness, Float temperature, Float pressure) throws Exception {
        return mockMvc.perform(
                put("/child/" + childid + "/measurements/")
                        .content(mapper.writeValueAsString(new PutData(humidity, brightness, temperature, pressure)))
                        .contentType(MediaType.APPLICATION_JSON)
                        .accept(MediaType.APPLICATION_JSON));
    }
}