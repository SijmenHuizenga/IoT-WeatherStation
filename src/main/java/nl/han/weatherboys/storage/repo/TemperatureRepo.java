package nl.han.weatherboys.storage.repo;

import nl.han.weatherboys.storage.model.Temperature;
import org.springframework.data.repository.CrudRepository;

public interface TemperatureRepo extends CrudRepository<Temperature, Integer> {
}
