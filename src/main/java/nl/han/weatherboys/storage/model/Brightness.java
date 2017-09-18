package nl.han.weatherboys.storage.model;

import javax.persistence.*;

@Entity
public class Brightness {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    public Integer id;

    @ManyToOne(optional = false)
    private Child child;

    public long moment;

    public float brightness;

    public Brightness(Child child, long moment, float brightness) {
        this.child = child;
        this.moment = moment;
        this.brightness = brightness;
    }

    public Brightness() {
    }
}
