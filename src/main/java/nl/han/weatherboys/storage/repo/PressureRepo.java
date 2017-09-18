package nl.han.weatherboys.storage.repo;

import nl.han.weatherboys.storage.model.Pressure;
import org.springframework.data.repository.CrudRepository;

public interface PressureRepo extends CrudRepository<Pressure, Integer> {
}
