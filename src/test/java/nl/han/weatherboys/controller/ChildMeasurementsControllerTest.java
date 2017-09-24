package nl.han.weatherboys.controller;

import com.fasterxml.jackson.databind.ObjectMapper;
import nl.han.weatherboys.Applications;
import nl.han.weatherboys.JpaConfig;
import nl.han.weatherboys.dto.PutData;
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
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.core.env.Environment;
import org.springframework.http.MediaType;
import org.springframework.jdbc.datasource.DriverManagerDataSource;
import org.springframework.test.annotation.Rollback;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.test.context.web.AnnotationConfigWebContextLoader;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.transaction.annotation.EnableTransactionManagement;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.context.WebApplicationContext;

import javax.annotation.Resource;
import javax.sql.DataSource;

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

    private Logger LOGGER = Logger.getLogger(this.getClass());

    private MockMvc mockMvc;
    private ObjectMapper mapper = new ObjectMapper();

    @Resource
    private ChildRepo childRepo;

    @Resource
    private JorgApiCredentialRepo credentialRepo;

    @Autowired
    private WebApplicationContext webApplicationContext;

    @Before
    public void setup() throws Exception {
        this.mockMvc = webAppContextSetup(webApplicationContext).build();
        credentialRepo.save(new JorgApiCredential(
                1, "autotest@example.com", "123abC*", "http://iot.jorgvisch.nl"
        ));
    }

    @Test
    @Rollback
    public void registerMeasurementAll() throws Exception {
        Child child = childRepo.save(new Child("UNITTEST"));

        mockMvc.perform(put("/child/"+child.id+"/measurements/")
                .content(mapper.writeValueAsString(new PutData(50f, 30f, 23f, 20f)))
                .contentType(MediaType.APPLICATION_JSON))
                .andDo(mvcResult -> LOGGER.trace("Response body: " + mvcResult.getResponse().getContentAsString()))
                .andExpect(status().isOk());
    }

}