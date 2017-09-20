package nl.han.weatherboys.storage.repo;

import nl.han.weatherboys.storage.model.JorgApiCredential;
import org.springframework.data.repository.CrudRepository;

public interface JorgApiCredentialRepo extends CrudRepository<JorgApiCredential, Integer> {
}
