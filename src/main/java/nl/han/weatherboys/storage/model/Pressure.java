package nl.han.weatherboys.storage.model;

import com.fasterxml.jackson.annotation.JsonProperty;
import io.swagger.annotations.ApiModelProperty;

import javax.persistence.*;

@Entity
public class Pressure {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
    public Integer id;

    @ManyToOne(optional = false)
    private Child child;

    @ApiModelProperty(notes = "The moment of this measurement in unix timestamp seconds.", required = true)
    public long moment;

    @ApiModelProperty(notes = "The humidity measurement value.", required = true)
    public float pressure;

    public Pressure(Child child, long moment, float pressure) {
        this.child = child;
        this.moment = moment;
        this.pressure = pressure;
    }

    public Pressure() {
    }
}
