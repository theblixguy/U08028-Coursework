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

#include <string>
#include "city.h"
#include "simple_vector.h"

double degree_to_radian(double value);
double calc_distance_cities(std::shared_ptr<City> &city1, std::shared_ptr<City> &city2);
double calc_distance_cities(double latitude, double longitude, std::shared_ptr<City> &city2);
double clear_screen();
bool isAlphaString(std::string str);
SimpleVector<std::string> split_line(char separator, const std::string &line);

#endif