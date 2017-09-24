package nl.han.weatherboys.storage.model;

import javax.persistence.*;
import javax.validation.constraints.Size;
import java.util.Set;

@Entity
public class Child {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    public Integer id;

    public String name;

    @OneToMany(fetch = FetchType.LAZY, mappedBy = "child")
    public Set<Brightness> brightnesses;

    @OneToMany(fetch = FetchType.LAZY, mappedBy = "child")
    public Set<Humidity> humidities;

    @OneToMany(fetch = FetchType.LAZY, mappedBy = "child")
    public Set<Pressure> pressures;

    @OneToMany(fetch = FetchType.LAZY, mappedBy = "child")
    public Set<Temperature> temperatures;

    public Child(String name) {
        this.name = name;
    }

    public Child() {
    }
}
