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

/* Function to set the Latitude of the coordinate */
void Coordinate::setLatitude(double lat) {
	latitude = lat;
}

/* Function to set the Longitude of the coordinate */
void Coordinate::setLongitude(double lon) {
	longitude = lon;
}

/* Function to set the Latitude and Latitude of the coordinate */
void Coordinate::setCoords(double lat, double lon) {
	latitude = lat;
	longitude = lon;
}