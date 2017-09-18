package nl.han.weatherboys.storage.model;

import javax.persistence.*;

@Entity
public class Temperature {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    public Integer id;

    @ManyToOne(optional = false)
    private Child child;

    public long moment;

    public float temperature;

    public Temperature(Child child, long moment, float temperature) {
        this.child = child;
        this.moment = moment;
        this.temperature = temperature;
    }

    public Temperature() {
    }
}
