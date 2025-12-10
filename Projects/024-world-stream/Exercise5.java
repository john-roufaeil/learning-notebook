import java.util.Comparator;
// import java.util.function.Function;

public class Exercise5 {
    public static void main(String[] args) {
        CountryDao countryDao = InMemoryWorldDao.getInstance();
        CityDao cityDao = InMemoryWorldDao.getInstance();

        countryDao.findAllCountries().stream()
            .map(country -> cityDao.findCityById(country.getCapital()))
            // .map(new Function<Country, City>() {
            //     @Override
            //     public City apply(Country country){
            //         return cityDao.findCityById(country.getCapital());
            //     }
            // })
            .filter(capital -> capital != null)
            // .filter(new Predicate<City>() {
            //     @Override
            //     public boolean test(City city) {
            //         return city != null;
            //     }
            // })
            .max(Comparator.comparingInt(City::getPopulation))
            // .max(Comparator.comparingInt(city -> city.getPopulation()))
            // .max((city1, city2) -> city1.getPopulation() - city2.getPopulation())
            .ifPresent(city -> 
                System.out.println(
                    "Most populated capital city is " + city.getName() + 
                    " in " + countryDao.findCountryByCode(city.getCountryCode()).getName() +
                    " with population of " + city.getPopulation()
                )); 
    }
}
