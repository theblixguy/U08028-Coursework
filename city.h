/* 
* City.h
* Contains definitions for the City class and getter functions
*
* Written by: Suyash Srijan
* Student number: 14076594
*
*/

#ifndef CITY_H
#define CITY_H
#include <string>

/* Simplify unique_ptr decl */
template <typename T>
using SPtr = typename std::shared_ptr<T>;
 
class City
{
private:
    double city_longitude;
    double city_latitude;
    std::string city_name;
    std::string city_country;
 
public:
    City(double latitude, double longitude, std::string city, std::string country);
 
    void setCity(double latitude, double longitude, std::string city, std::string country);
    double getDistanceTo(SPtr<City> &target_city);
 
    double getLatitude() { return city_latitude; }
    double getLongitude() { return city_longitude; }
    std::string getCity() { return city_name; }
    std::string getCityCountry() { return city_country; }
};
 
#endif