// import java.util.List;
// import java.util.stream.Collectors;
// import java.util.Comparator;
// import java.util.Optional;

public class Exercise1 {

   public static void main(String[] args) {
      CountryDao countryDao= InMemoryWorldDao.getInstance();
      // StringBuilder outputString = new StringBuilder();
      // countryDao
      //    .findAllCountries()
      //    .stream()
      //    .forEach(
      //       country -> {
      //          outputString.append("Country: ").append(country.getName()).append("\n");
      //          Optional<City> maxCity = country
      //             .getCities()
      //             .stream()
      //             .max(Comparator.comparingInt(City::getPopulation));
      //          outputString.append(maxCity.get().getName()).append(" ").append(maxCity.get().getPopulation());
      //          outputString.append("--------\n");
      //       }
      //    );
      // System.out.println(outputString);
   }
}
