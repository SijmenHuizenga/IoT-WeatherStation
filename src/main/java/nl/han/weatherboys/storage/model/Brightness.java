package nl.han.weatherboys.storage.model;

import com.fasterxml.jackson.annotation.JsonProperty;
import io.swagger.annotations.ApiModelProperty;

import javax.persistence.*;

@Entity
public class Brightness {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
    public Integer id;

    @ManyToOne(optional = false)
    private Child child;

    @ApiModelProperty(notes = "The moment of this measurement in unix timestamp seconds.", required = true)
    public long moment;

    @ApiModelProperty(notes = "The brightness measurement value in percentage between 0.0 to 100.0", required = true)
    public float brightness;

    public Brightness(Child child, long moment, float brightness) {
        this.child = child;
        this.moment = moment;
        this.brightness = brightness;
    }

    public Brightness() {
    }
}
