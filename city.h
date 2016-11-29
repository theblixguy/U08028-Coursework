/* 
* city.h
* Contains definitions for the City class and getter functions
*
* Written by: Suyash Srijan
* Student number: 14076594
*
*/

#ifndef CITY_H
#define CITY_H
#include <string>
#include "coordinate.h"
#include <memory>

/* Simplify shared_ptr decl */
template <typename T>
using SPtr = typename std::shared_ptr<T>;
 
class City
{
private:
    SPtr<Coordinate> city_coords;
    std::string city_name;
    std::string city_country;
 
public:
    City(double latitude, double longitude, std::string const& city, std::string const& country);
    void setCity(double latitude, double longitude, std::string const& city, std::string const& country);
    void setCityCoords(double lat, double lon);
    void setCityAndCountry(std::string const& city, std::string const& country);
    double getDistanceTo(SPtr<City> &target_city);
    Coordinate *getCityCoords() { return city_coords.get(); }
    double getLatitude() { return city_coords->getLatitude(); }
    double getLongitude() { return city_coords->getLongitude(); }
    std::string getCity() { return city_name; }
    std::string getCityCountry() { return city_country; }
};
 
#endif