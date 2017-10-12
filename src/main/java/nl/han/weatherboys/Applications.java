package nl.han.weatherboys;

import com.google.common.base.Predicates;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.context.PropertyPlaceholderAutoConfiguration;
import org.springframework.boot.autoconfigure.jackson.JacksonAutoConfiguration;
import org.springframework.boot.autoconfigure.web.*;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Import;
import springfox.documentation.builders.RequestHandlerSelectors;
import springfox.documentation.service.ApiInfo;
import springfox.documentation.service.Tag;
import springfox.documentation.spi.DocumentationType;
import springfox.documentation.spring.web.plugins.Docket;
import springfox.documentation.swagger2.annotations.EnableSwagger2;

import java.util.Collections;

import static springfox.documentation.builders.PathSelectors.ant;

@SpringBootApplication
@ComponentScan
@EnableSwagger2
@Configuration
public class Applications {

    public static void main(String[] args) {
        SpringApplication.run(Applications.class, args);
    }

    @Bean
    public Docket api() {
        return new Docket(DocumentationType.SWAGGER_2)
                .select()
                  .apis(RequestHandlerSelectors.any())
                  .paths(Predicates.and(ant("/**"), Predicates.not(ant("/error"))))
                  .build()
                .pathMapping("/")
                .apiInfo(apiInfo())
                .tags(new Tag("Time", ""))
                .tags(new Tag("Children", ""))
                .useDefaultResponseMessages(false)
                .consumes(Collections.singleton("application/json"))
                .produces(Collections.singleton("application/json"));
    }

    private ApiInfo apiInfo() {
        return new ApiInfo(
                "Weatherboys Gateway",
                "The gateway rest api for the weatherboys weathersystem.",
                "v1.0",
                null,
                null,
                "MIT License",
                "https://github.com/SijmenHuizenga/IoT-WeatherStation/blob/master/LICENSE",
                Collections.emptyList()
        );
    }

}

