/* 
* Coordinate.h
* Contains definitions for the Coordinate class and getter functions
*
* Written by: Suyash Srijan
* Student number: 14076594
*
*/

#ifndef COORDINATE_H
#define COORDINATE_H
#include <string>
 
class Coordinate
{
private:
    double coord_longitude;
    double coord_latitude;
    std::string coord_city;
 
public:
    Coordinate(double latitude, double longitude, std::string city);
 
    void setCoordinate(double latitude, double longitude, std::string city);
 
    double getLatitude() { return coord_latitude; }
    double getLongitude() { return coord_longitude; }
    std::string getCity() { return coord_city; }
};
 
#endif