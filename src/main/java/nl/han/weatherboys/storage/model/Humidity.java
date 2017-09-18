package nl.han.weatherboys.storage.model;

import javax.persistence.*;

/**
 * A measurement of the air humidity from a weather station at a specific timestamp.
 */
@Entity
public class Humidity {

    public Humidity(Station station, long moment, float humidity) {
        this.station = station;
        this.moment = moment;
        this.humidity = humidity;
    }

    public Humidity() {
    }

    /**
     * An unique id to reference this measurement
     */
    @Id
    @GeneratedValue(strategy= GenerationType.AUTO)
    private Integer id;

    /**
     * The station that measured this measurement
     */
    @ManyToOne
    private Station station;

    /**
     * Timestamp of the measurement in unix timestamp seconds
     */
    private long moment;

    /**
     * The percentage humidity measured ath the given moment
     */
    private float humidity;


}
