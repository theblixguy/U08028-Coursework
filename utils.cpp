/*
* utils.cpp
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
#include "simple_vector.h"

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

/* Pi constant */
#define PI 3.14159265358979323846264338327950288

/* Constants that define the Earth ellipsoid WGS84 */
#define A_AXIS 6378137.0
#define B_AXIS 6356752.31
#define FLATTENING 0.00335281066

/* More constants for Vincenty formula implementation */
#define U_CONST 0.99664718934 // 1 - FLATTENING

/* Function to clear the console screen. It's a combination of two ANSI escape codes, \033[2J to clear
* the console screen from top to bottom and, \033[1;1H to place the cursor at 1,1 (row, column).
* On Windows, it uses the console API to clear the screen programatically, which is much better than
* using std::system("cls"); for obvious reasons
*/
void clear_screen() {
#ifdef _WIN32
	HANDLE stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = {0, 0};
	DWORD count;
	CONSOLE_SCREEN_BUFFER_INFO screenBufInf;
	GetConsoleScreenBufferInfo(stdOutHandle, &screenBufInf);
	FillConsoleOutputCharacter(stdOutHandle, ' ', screenBufInf.dwSize.X * screenBufInf.dwSize.Y, coord, &count);
	SetConsoleCursorPosition(stdOutHandle, coord);
#else
	std::cout << "\033[2J\033[1;1H";
#endif
}

/* Overcomplicated function to determine if you're running on Windows 10 v1511 (Threshold 2), which is required to
* enable Virtual terminal sequences on Windows console. The reason why this function is being used is because
* GetVersionEx() is now deprecated and using the helper function IsWindows10OrGreater() from VersionHelpers.h
* requires a special manifest file for the program, otherwise it returns a different version number, which
* is more messy to write and add to makefile, so it's just simpler to read the releaseId from the Windows
* registery and check whether it's at least 1511, which means TH2, which has support for Virtual terminal
* equences.
*/
#ifdef _WIN32
bool IsRunningWin10TH2() {
	HKEY releaseIdKey;
	int id;
	DWORD valueType;
	BYTE buffer[256];
	DWORD bufferSize = 256;
	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", 0, KEY_QUERY_VALUE, &releaseIdKey) != ERROR_SUCCESS) return false;
	if (RegQueryValueExW(releaseIdKey, L"ReleaseId", nullptr, &valueType, buffer, &bufferSize) != ERROR_SUCCESS) return false;
	if (valueType != REG_SZ) return false;
	std::wistringstream idStream(reinterpret_cast<wchar_t*>(buffer));
	idStream >> id;
	RegCloseKey(releaseIdKey);
	return id >= 1511;
}
#endif

/* Method to enable Virtual terminal sequences on Windows console using the Console API (requires at least TH2)*/
#ifdef _WIN32
void enableEscapeSequencesOnConsole() {
	HANDLE stdOutHandle;
	DWORD consoleMode;
	stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	consoleMode = ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT;
	SetConsoleMode(stdOutHandle, consoleMode);
	consoleMode = 0;
	if (!GetConsoleMode(stdOutHandle, &consoleMode)) {
		std::cout << "Error enabling Virtual Terminal Processing" << std::endl;
	}
}
#endif

/* Comparator for the caseInsensitiveCompare() function */
bool caseInsensitiveComparator(unsigned char ch1, unsigned char ch2) {
	return std::tolower(ch1) == std::tolower(ch2);
}

/* Function to compare two strings while ignoring their case */
bool caseInsensitiveCompare(std::string const& str1, std::string const& str2) {
	if (str1.length() != str2.length()) return false;
	return std::equal(str2.begin(), str2.end(), str1.begin(), caseInsensitiveComparator);
}

/* Function to convert a degree value to radian value */
double degree_to_radian(double value) {
	return (value * PI) / 180;
}

/* Function to calculate the distance between two coordinates using Vincenty's formula
*  Based on JavaScript implementation found at http://movable-type.co.uk/scripts/latlong-vincenty-direct.html
*/
double calcDistance(double latitude1, double longitude1, double latitude2, double longitude2) {
	double U1 = atan(U_CONST * tan(degree_to_radian(latitude1)));
	double U2 = atan(U_CONST * tan(degree_to_radian(latitude2)));
	double sinU1 = sin(U1), cosU1 = cos(U1);
	double sinU2 = sin(U2), cosU2 = cos(U2);
	double latDiff = degree_to_radian(longitude2 - longitude1);

	double sigma;
	double sinLambda;
	double sinSigma;
	double cosSigma;
	double cosLambda;
	double cosSqAlpha;
	double cos2SigmaM;

	double lambda = latDiff;
	double prevLambda;
	int iterLim = 100;

	do {
		sinLambda = sin(lambda);
		cosLambda = cos(lambda);;
		sinSigma = sqrt(pow(cosU2 * sinLambda, 2) + pow(cosU1 * sinU2 - sinU1 * cosU2 * cosLambda, 2));

		if (sinSigma == 0)
			return 0;

		cosSigma = sinU1 * sinU2 + cosU1 * cosU2 * cosLambda;
		sigma = atan(sinSigma / cosSigma);
		double sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
		cosSqAlpha = 1 - pow(sinAlpha, 2);

		if (cosSqAlpha == 0)
			cos2SigmaM = 0;
		else
			cos2SigmaM = cosSigma - 2 * sinU1 * sinU2 / cosSqAlpha;

		double C = FLATTENING / 16 * cosSqAlpha * (4 + FLATTENING * (4 - 3 * cosSqAlpha));
		prevLambda = lambda;
		lambda = latDiff + (1 - C) * FLATTENING * sinAlpha * (sigma + C * sinSigma * (cos2SigmaM + C * cosSigma * (-1 + 2 * pow(cos2SigmaM, 2))));
	} while (std::abs(lambda - prevLambda) > 1e-12 && --iterLim > 0);

	if (iterLim == 0)
		return 0;

	double uSq = cosSqAlpha * (pow(A_AXIS, 2) - pow(B_AXIS, 2)) / pow(B_AXIS, 2);
	double A = 1 + uSq / 16384 * (4096 + uSq * (-768 + uSq * (320 - 175 * uSq)));
	double B = uSq / 1024 * (256 + uSq * (-128 + uSq * (74 - 47 * uSq)));
	double deltaSigma = B * sinSigma * (cos2SigmaM + B / 4 * (cosSigma * (-1 + 2 * pow(cos2SigmaM, 2)) - B / 6 * cos2SigmaM * (-3 + 4 * pow(sinSigma, 2)) * (-3 + 4 * pow(cos2SigmaM, 2))));

	double dist = B_AXIS * A * (sigma - deltaSigma);
	return dist / 1000;
}

/* Function to calculate the distance between two latitudes and two longitudes
*/
double calc_distance_cities(std::shared_ptr<City> &city1, std::shared_ptr<City> &city2) {
	return calcDistance(city1->getLatitude(), city1->getLongitude(), city2->getLatitude(), city2->getLongitude());
}

/* Same as above, but accepts a City object to get its coords */
double calc_distance_cities(std::shared_ptr<Coordinate> &coord1, std::shared_ptr<City> &target_city) {
	return calcDistance(coord1->getLatitude(), coord1->getLongitude(), target_city->getLatitude(), target_city->getLongitude());
}

/* Function to check if a string is comprised of only alphabets */
bool isAlphaString(std::string str) {
	return std::regex_match(str, std::regex("^[A-Za-z ]+$"));
}

/* Tokenize a comma separated string and return it in a vector */
SimpleVector<std::string> split_line(char separator, const std::string &line) {
	SimpleVector<std::string> tokens;
	std::string::size_type begin = 0;
	std::string::size_type end = 0;
	while ((end = line.find(separator, begin)) != std::string::npos) {
		tokens.push_back(line.substr(begin, end - begin));
		begin = end + 1;
	}
	tokens.push_back(line.substr(begin));
	return tokens;
}