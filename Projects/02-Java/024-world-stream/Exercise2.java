import java.util.Comparator;

public class Exercise2 {
    public static void main(String[] args) {
        CountryDao countryDao = InMemoryWorldDao.getInstance();
        countryDao.getAllContinents().stream().forEach(continent -> {
            countryDao.findCountriesByContinent(continent).stream()
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
