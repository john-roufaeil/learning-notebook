

public class Exercise1 {

   public static void main(String[] args) {
      CountryDao countryDao= InMemoryWorldDao.getInstance();
      // countryDao.findAllCountries().stream().forEach(country -> {
      //    String countryName = country.getName();
      //    List<City> countryCities = country.getCities();

      //    countryCities.stream()
      //       .max(Comparator.comparingInt(City::getPopulation))
      //       .ifPresent(city -> System.out.println(
      //          city.getName() + " in " + countryName + 
      //          " has population of " + city.getPopulation()
      //       ));
      // });
   }
}

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


//  countryDao.findAllCountries().stream().forEach(new Consumer<Country>() {
         //     @Override
         //     public void accept(Country country) {
         //         String countryName = country.getName();
         //         List<City> countryCities = country.getCities();
         //     }
         // }
         
         // Optional<City> maxPopulatedCity = countryCities.stream()
         //          .max(Comparator.comparingInt(City::getPopulation));
         // if (maxPopulatedCity.isPresent()) {
         //    System.out.println(
         //       maxPopulatedCity.get().getName() + " in "  + 
         //       countryName + " has population of " + 
         //       maxPopulatedCity.get().getPopulation()
         //    );
         // }