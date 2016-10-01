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
#include "coordinate.h"

double degree_to_radian(double value);
double calc_distance_coords(std::unique_ptr<Coordinate> &coord1, std::unique_ptr<Coordinate> &coord2);
double clear_screen();

#endif