package nl.han.weatherboys.controller;

import nl.han.weatherboys.Applications;
import nl.han.weatherboys.jorgapi.JorgApi;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;

import java.io.IOException;

import static org.hamcrest.core.IsEqual.equalTo;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;
import static org.springframework.test.web.servlet.setup.MockMvcBuilders.standaloneSetup;

@RunWith(SpringRunner.class)
@SpringBootTest(classes = Applications.class)
@WebAppConfiguration
public class TimeControllerTest {

    private MockMvc mockMvc;

    @Mock
    private JorgApi jorgApiMock;

    @InjectMocks
    private TimeController controller;

    @Before
    public void setup() throws Exception {
        MockitoAnnotations.initMocks(this);

        this.mockMvc = standaloneSetup(controller).build();
    }

    @Test
    public void now() throws Exception {
        when(jorgApiMock.getTime()).thenReturn(500L);

        mockMvc.perform(get("/time/now"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.time", equalTo(500)));
    }

    @Test
    public void nowErrorFromJorgApi() throws Exception {
        when(jorgApiMock.getTime()).thenThrow(new IOException());

        mockMvc.perform(get("/time/now"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.time", equalTo((int) (System.currentTimeMillis()/1000))));
    }

}