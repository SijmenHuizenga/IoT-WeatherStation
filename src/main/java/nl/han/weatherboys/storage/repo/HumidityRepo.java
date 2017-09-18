package nl.han.weatherboys.storage.repo;

import nl.han.weatherboys.storage.model.Humidity;
import org.springframework.data.repository.CrudRepository;

public interface HumidityRepo extends CrudRepository<Humidity, Integer> {

}