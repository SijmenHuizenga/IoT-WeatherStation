package nl.han.weatherboys.storage.repo;

import nl.han.weatherboys.storage.model.Brightness;
import org.springframework.data.repository.CrudRepository;

public interface BrightnessRepo extends CrudRepository<Brightness, Integer> {
}
