package nl.han.weatherboys.storage.model;

import io.swagger.annotations.ApiModelProperty;
import org.hibernate.validator.constraints.Length;

import javax.persistence.*;
import java.util.Set;

@Entity
public class Child {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @ApiModelProperty(notes = "Identifier of this child", required = true)
    public Integer id;

    @Length(min=3, max=5)
    @ApiModelProperty(notes = "Name of this child. 3 to 5 characters", required = true)
    public String name;

    @ApiModelProperty(notes = "Last known IP address of this child.", required = true)
    public String ip;

    @ApiModelProperty(notes = "All brightness measurements received from this child.", required = true, position = 1)
    @OneToMany(mappedBy = "child")
    @OrderBy("moment")
    public Set<Brightness> brightnesses;

    @ApiModelProperty(notes = "All humiditie measurements received from this child.", required = true, position = 1)
    @OneToMany(mappedBy = "child")
    @OrderBy("moment")
    public Set<Humidity> humidities;

    @ApiModelProperty(notes = "All pressure measurements received from this child.", required = true, position = 1)
    @OneToMany(mappedBy = "child")
    @OrderBy("moment")
    public Set<Pressure> pressures;

    @ApiModelProperty(notes = "All temperature measurements received from this child.", required = true, position = 1)
    @OneToMany(mappedBy = "child")
    @OrderBy("moment")
    public Set<Temperature> temperatures;

    public Child(String name) {
        this.name = name;
    }

    public Child() {
    }
}
