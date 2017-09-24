package nl.han.weatherboys.controller;

import com.fasterxml.jackson.databind.ObjectMapper;
import nl.han.weatherboys.Applications;
import nl.han.weatherboys.JpaConfig;
import nl.han.weatherboys.dto.PutData;
import nl.han.weatherboys.jorgapi.ResetTokenService;
import nl.han.weatherboys.storage.model.Child;
import nl.han.weatherboys.storage.model.JorgApiCredential;
import nl.han.weatherboys.storage.repo.ChildRepo;
import nl.han.weatherboys.storage.repo.JorgApiCredentialRepo;
import org.apache.log4j.Logger;
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
import org.springframework.web.context.WebApplicationContext;

import javax.annotation.Resource;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.put;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;
import static org.springframework.test.web.servlet.setup.MockMvcBuilders.webAppContextSetup;

@RunWith(SpringRunner.class)
@SpringBootTest(classes = Applications.class)
@WebAppConfiguration
@ContextConfiguration(
        classes = { JpaConfig.class },
        loader = AnnotationConfigWebContextLoader.class)
public class ChildMeasurementsControllerTest {

    private static final String USER = "autotest@example.com", PASS = "123abC*", URL = "http://iot.jorgvisch.nl";

    private Logger LOGGER = Logger.getLogger(this.getClass());

    private MockMvc mockMvc;
    private ObjectMapper mapper = new ObjectMapper();

    @Resource
    private ChildRepo childRepo;

    @Resource
    private JorgApiCredentialRepo credentialRepo;

    @Autowired
    private WebApplicationContext webApplicationContext;

    @Autowired
    private ResetTokenService resetTokenService;

    @Before
    public void setup() throws Exception {
        this.mockMvc = webAppContextSetup(webApplicationContext).build();
        credentialRepo.save(new JorgApiCredential(1, USER, PASS, URL));
        resetTokenService.resetToken(USER, PASS, URL);
    }

    @Test
    @Rollback
    public void registerMeasurementAll() throws Exception {
        Child child = childRepo.save(new Child("UTEST"));

        mockMvc.perform(put("/child/"+child.id+"/measurements/")
                .content(mapper.writeValueAsString(new PutData(50f, 30f, 23f, 20f)))
                .contentType(MediaType.APPLICATION_JSON))
                .andDo(mvcResult -> LOGGER.trace("Response body: " + mvcResult.getResponse().getContentAsString()))
                .andExpect(status().isOk());
    }

}