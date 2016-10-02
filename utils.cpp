/* 
* Utils.cpp
* Contains helper functions for the Cities program
*
* Written by: Suyash Srijan
* Student number: 14076594
*
*/

/* Imports we need */
#include <cmath>
#include <iostream>
#include <memory>
#include <regex>
#include "city.h"

/* Need to define this macro to access the macro definitions for common math constants */
#define _USE_MATH_DEFINES

/* Macro that contains the earth's radius in kilometers */
#define EARTH_RADIUS_KM 6371.0

/* Function to clear the console screen. It's a combination of two ANSI escape codes, \033[2J to clear
* the console screen from top to bottom and, \033[1;1H to place the cursor at 1,1 (row, column)
*/
void clear_screen() {
	std::cout << "\033[2J\033[1;1H";
}

/* Function to convert a degree value to radian value */
double degree_to_radian(double value) {
	return (value * M_PI) / 180;
}

/* Function to calculate the distance between two latitudes and two longitudes using Haversine formula 
*  https://en.wikipedia.org/wiki/Haversine_formula
*/
double calc_distance_cities(std::unique_ptr<City> &city1, std::unique_ptr<City> &city2) {
	double lat1_rad, lon1_rad, lat2_rad, lon2_rad, x, y;
	lat1_rad = degree_to_radian(city1->getLatitude());
	lon1_rad = degree_to_radian(city1->getLongitude());
	lat2_rad = degree_to_radian(city2->getLatitude());
	lon2_rad = degree_to_radian(city2->getLongitude());
	x = sin((lat2_rad - lat1_rad) / 2);
	y = sin((lon2_rad - lon1_rad) / 2);
	return 2.0 * EARTH_RADIUS_KM * asin(sqrt(x * x + cos(lat1_rad) * cos(lat2_rad) * y * y));
}

/* Same as above, but accepts coords of one City directly */
double calc_distance_cities(double latitude, double longitude, std::unique_ptr<City> &target_city) {
	double lat1_rad, lon1_rad, lat2_rad, lon2_rad, x, y;
	lat1_rad = degree_to_radian(latitude);
	lon1_rad = degree_to_radian(longitude);
	lat2_rad = degree_to_radian(target_city->getLatitude());
	lon2_rad = degree_to_radian(target_city->getLongitude());
	x = sin((lat2_rad - lat1_rad) / 2);
	y = sin((lon2_rad - lon1_rad) / 2);
	return 2.0 * EARTH_RADIUS_KM * asin(sqrt(x * x + cos(lat1_rad) * cos(lat2_rad) * y * y));
}

/* Function to check if a string is comprised of only alphabets */
bool isAlphaString(std::string str) {
	return std::regex_match(str, std::regex("^[A-Za-z]+$"));
}