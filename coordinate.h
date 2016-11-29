/* 
* coordinate.h
* Contains definitions for the Coordinate class and getter functions
*
* Written by: Suyash Srijan
* Student number: 14076594
*
*/

#ifndef COORDINATE_H
#define COORDINATE_H

class Coordinate {

private:
	double longitude;
	double latitude;

public:
	Coordinate(double lat, double lon);
	double getLatitude() { return latitude; }
	double getLongitude() { return longitude; }
	void setLatitude(double lat);
	void setLongitude(double lon);
	void setCoords(double lat, double lon);
};
#endif