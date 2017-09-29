package nl.han.weatherboys.storage.model;

import com.fasterxml.jackson.annotation.JsonProperty;
import io.swagger.annotations.ApiModelProperty;

import javax.persistence.*;

@Entity
public class Humidity {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
    public Integer id;

    @ManyToOne(optional = false)
    private Child child;

    @ApiModelProperty(notes = "The moment of this measurement in unix timestamp seconds.", required = true)
    public long moment;

    @ApiModelProperty(notes = "The humidity measurement value in percentage between 0.0 to 80.0", required = true)
    public float humidity;

    public Humidity(Child child, long moment, float humidity) {
        this.child = child;
        this.moment = moment;
        this.humidity = humidity;
    }

    public Humidity() {
    }

}
