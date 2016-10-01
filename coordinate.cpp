/* 
* Coordinate.cpp
* A class describing a Coordinate along with the name of place it points to
*
* Written by: Suyash Srijan
* Student number: 14076594
*
*/

#include "coordinate.h"

/* Constructor for the class */
Coordinate::Coordinate(double latitude, double longitude, std::string city)
{
    setCoordinate(latitude, longitude, city);
}
 
/* Function that sets the coordinate values and the city name */
void Coordinate::setCoordinate(double latitude, double longitude, std::string city)
{
    coord_latitude = latitude;
    coord_longitude = longitude;
    coord_city = city;
}