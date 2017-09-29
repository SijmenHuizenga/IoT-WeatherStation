package nl.han.weatherboys.storage.model;

import com.fasterxml.jackson.annotation.JsonProperty;
import io.swagger.annotations.ApiModelProperty;

import javax.persistence.*;

@Entity
public class Temperature {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
    public Integer id;

    @ManyToOne(optional = false)
    private Child child;

    @ApiModelProperty(notes = "The moment of this measurement in unix timestamp seconds.", required = true)
    public long moment;

    @ApiModelProperty(notes = "The humidity measurement value in percentage between 10.0 and 85.0 degrees celsius", required = true)
    public float temperature;

    public Temperature(Child child, long moment, float temperature) {
        this.child = child;
        this.moment = moment;
        this.temperature = temperature;
    }

    public Temperature() {
    }
}
