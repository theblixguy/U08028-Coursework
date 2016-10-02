/* 
* Utils.h
* Contains definitions of helper functions for the Cities program
*
* Written by: Suyash Srijan
* Student number: 14076594
*
*/

#ifndef UTILS_H
#define UTILS_H
#include "city.h"
#include <string>

double degree_to_radian(double value);
double calc_distance_cities(std::unique_ptr<City> &city1, std::unique_ptr<City> &city2);
double calc_distance_cities(double latitude, double longitude, std::unique_ptr<City> &city2);
double clear_screen();
bool isAlphaString(std::string str);
bool isDouble(double value){
	
}

#endif