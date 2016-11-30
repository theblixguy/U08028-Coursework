/*
* city.cpp
* A class describing a City
*
* Written by: Suyash Srijan
* Student number: 14076594
*
*/

#include "city.h"
#include "utils.h"

/* Constructor for the class */
City::City(double latitude, double longitude, std::string const& city, std::string const& country) {
	setCity(latitude, longitude, city, country);
}

/* Function that sets the coordinate values, city name and the city country */
void City::setCity(double latitude, double longitude, std::string const& city, std::string const& country) {
	city_coords = std::make_shared<Coordinate>(latitude, longitude);
	city_name = city;
	city_country = country;
}

/* Function that modifies the coordinates of the city */
void City::setCityCoords(double lat, double lon) {
	city_coords->setCoords(lat, lon);
}

/* Function to set the City and Country name of the City object */
void City::setCityAndCountry(std::string const& city, std::string const& country) {
	city_name = city;
	city_country = country;
}

/* Function that gets the distance from current coords to another city (coords) */
double City::getDistanceTo(SPtr<City> &target_city) {
	return calc_distance_cities(city_coords, target_city);
}