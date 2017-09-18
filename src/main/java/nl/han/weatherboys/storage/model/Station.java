package nl.han.weatherboys.storage.model;

import javax.persistence.*;
import java.util.Set;

/**
 * Represents a physical weather station
 */
@Entity
public class Station {

    /**
     * An unique id to reference this weather station
     */
    @Id
    @GeneratedValue(strategy= GenerationType.AUTO)
    public Integer id;

    /**
     * The human understandable name of this station
     */
    public String name;

    /**
     * All humidity measurements made by this station
     */
    @OneToMany(fetch = FetchType.LAZY, mappedBy = "station")
    public Set<Humidity> humidities;

}
