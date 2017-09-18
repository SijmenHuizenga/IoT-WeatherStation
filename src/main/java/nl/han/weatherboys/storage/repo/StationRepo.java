package nl.han.weatherboys.storage.repo;

import nl.han.weatherboys.storage.model.Station;
import org.springframework.data.repository.CrudRepository;

public interface StationRepo extends CrudRepository<Station, Integer> {

}