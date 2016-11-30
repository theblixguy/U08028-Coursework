/*
* utils.h
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
double calcDistance(double latitutde1, double longitude1, double latitude2, double longitude2);
double calc_distance_cities(std::shared_ptr<City> &city1, std::shared_ptr<City> &city2);
double calc_distance_cities(std::shared_ptr<Coordinate> &coord1, std::shared_ptr<City> &city2);
double clear_screen();
bool IsRunningWin10TH2();
void enableEscapeSequencesOnConsole();
bool caseInsensitiveComparator(unsigned char ch1, unsigned char ch2);
bool caseInsensitiveCompare(std::string const& str1, std::string const& str2);
bool isAlphaString(std::string str);
SimpleVector<std::string> split_line(char separator, const std::string &line);

#endif