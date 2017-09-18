package nl.han.weatherboys.storage.repo;

import nl.han.weatherboys.storage.model.Child;
import org.springframework.data.repository.CrudRepository;

public interface ChildRepo extends CrudRepository<Child, Integer> {

}