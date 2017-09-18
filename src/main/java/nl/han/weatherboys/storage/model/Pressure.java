package nl.han.weatherboys.storage.model;

import javax.persistence.*;

@Entity
public class Pressure {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    public Integer id;

    @ManyToOne(optional = false)
    private Child child;

    public long moment;

    public float pressure;

    public Pressure(Child child, long moment, float pressure) {
        this.child = child;
        this.moment = moment;
        this.pressure = pressure;
    }

    public Pressure() {
    }
}
