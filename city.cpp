/* 
* City.cpp
* A class describing a City
*
* Written by: Suyash Srijan
* Student number: 14076594
*
*/

#include "city.h"
#include "utils.h"

/* Constructor for the class */
City::City(double latitude, double longitude, std::string city, std::string country) {
    setCity(latitude, longitude, city, country);
}
 
/* Function that sets the coordinate values and the city name */
void City::setCity(double latitude, double longitude, std::string city, std::string country) {
    city_latitude = latitude;
    city_longitude = longitude;
    city_name = city;
    city_country = country;
}

/* Function that gets the distance from current coords to another city (coords) */
double City::getDistanceTo(SPtr<City> &target_city) {
	return calc_distance_cities(city_latitude, city_longitude, target_city);
}