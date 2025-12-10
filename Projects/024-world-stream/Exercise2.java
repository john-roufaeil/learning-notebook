
import java.util.Comparator;
import java.util.List;

public class Exercise2 {

    public static void main(String[] args) {
        CountryDao countryDao = InMemoryWorldDao.getInstance();
        countryDao.getAllContinents().stream().forEach(continent -> {
            List<Country> countriesInContinent = countryDao.findCountriesByContinent(continent);
            countriesInContinent.stream()
                .flatMap(country -> country.getCities().stream())
                .max(Comparator.comparingInt(City::getPopulation))
                .ifPresent(city -> System.out.println(
                    "In " + continent + ", " + city.getName() + " in " + 
                    countryDao.findCountryByCode(city.getCountryCode()).getName() + 
                    " has population of " + city.getPopulation()
                )
            );
        });
    }
}
