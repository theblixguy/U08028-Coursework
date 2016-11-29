/* 
* coordinate.cpp
* A class describing a Coordinate
*
* Written by: Suyash Srijan
* Student number: 14076594
*
*/

#include "coordinate.h"

/* Constructor for the class */
Coordinate::Coordinate(double lat, double lon) {
	setCoords(lat, lon);
}

void Coordinate::setLatitude(double lat) {
	latitude = lat;
}

void Coordinate::setLongitude(double lon) {
	longitude = lon;
}

void Coordinate::setCoords(double lat, double lon) {
	latitude = lat;
	longitude = lon;
}