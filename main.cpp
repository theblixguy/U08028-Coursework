/*
*
* Cities program (U08028 Coursework)
* Written by: Suyash Srijan
* Student number: 14076594
*
* This code uses some C++ 11 and C++ 14 features like auto, std::make_unique, std::function, and lambdas 
* so please don't forget to add -std=c++14 to g++ when compiling!
* 
* To enable printing of performance stats (i.e time taken to sort & save/load cities data), add 
* -D PRINT_PERF_STATS to g++ or add $(PERF_STAT_FLAG) to main.o target in makefile!
*/

/* Imports we need */
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <array>
#include <functional>
#include <chrono>
#include "simple_vector.h"
#include "city.h"
#include "utils.h"

/* Macros to store the date/time to show the compilation date/time in program */
#define build_time __TIME__
#define build_date __DATE__

/* Macros to store ANSI escape codes for text customization */
#define CLR_NORMAL  "\x1B[0m"
#define CLR_RED     "\x1B[31m"
#define CLR_YELLOW  "\x1B[33m"
#define CLR_GREEN   "\x1B[32m"
#define CLR_CYAN    "\x1B[36m"
#define CLR_MAGENTA "\x1B[35m"
#define BOLD_ON     "\x1B[1m"
#define BOLD_OFF    "\x1B[21m"

/* Throw error if on Windows as there's some changes that need to be done to properly support it. 
* There are some minor changes that needs to be made in utils.cpp
*/
#ifdef _WIN32
#error Sorry, you cannot compile this program on Windows (yet)
#endif

/* Vector of unique_ptr's pointing to our custom City class objects */
SimpleVector<std::unique_ptr<City>> cities;

/* Function signatures */
void printProgramIntro();
void printProgramMenu();
void sortCities();
void addCity();
void displayAllCities();
void handleMenuChoice();
void modifyCity();
void deleteCity();
void findDistCities();
void exitProgram();
void loadCitiesData();
void saveCitiesData();
int getProgramMenuChoice();
auto cityExists();

/* Duh */
int main() {

#ifdef PRINT_PERF_STATS
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    loadCitiesData();
    std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
    std::cout << CLR_MAGENTA << "Info: Took " << duration << " microseconds to load cities data into vector" << CLR_NORMAL << std::endl;
#else
    loadCitiesData();
#endif

    printProgramIntro();
    printProgramMenu();
    handleMenuChoice();
    return 0;
}

/* Function to print some introduction text (program name, author name, compile date/time, etc) */
void printProgramIntro() {
    std::cout << CLR_CYAN << "------------------------------------------------------------" << std::endl;
    std::cout << BOLD_ON << "Cities program (U08028 Coursework) by Suyash Srijan" << std::endl;
    std::cout << "Compiled on: " << build_date << " / " << build_time << BOLD_OFF << std::endl;
    std::cout << "------------------------------------------------------------" << CLR_NORMAL << std::endl;
}

/* Function to print out program's menu */
void printProgramMenu() {
    std::cout << "\nPlease enter your choice\n\n0. Add a city\n1. Modify a city\n2. Delete a city\n3. Find distance between cities\n4. Display all cities\n5. Exit\n\n";
}

/* Function that reads the menu choice from standard input and returns it to the caller */
int getProgramMenuChoice() {
    int choice = 0;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

/* Function to sort our vector in ascending order (A-Z) */
void sortCities() {

#ifdef PRINT_PERF_STATS
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::sort(cities.begin(), cities.end(), [](const std::unique_ptr<City>& city1, const std::unique_ptr<City>& city2) {
        return city1->getCity() < city2->getCity() && city1->getCityCountry() < city2->getCityCountry(); 
    });
    std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
    std::cout << CLR_MAGENTA << "Info: Took " << duration << " microseconds to sort " << cities.size() << " cities" << CLR_NORMAL << std::endl;
#else
    std::sort(cities.begin(), cities.end(), [](const std::unique_ptr<City>& city1, const std::unique_ptr<City>& city2) {
        return city1->getCity() < city2->getCity() && city1->getCityCountry() < city2->getCityCountry(); 
    });
#endif

}

/* Function that loads data for cities from the disk into the vector */
void loadCitiesData() {
    std::ifstream data_file("cities_data.txt");

    if (!data_file.good()) {
        std::cout << CLR_YELLOW << BOLD_ON << "Warn: Database file missing or bad, a new one has been created\n" << BOLD_OFF << CLR_NORMAL << std::endl;
        std::ofstream outfile ("cities_data.txt");
        outfile.close();
        return;
    }
    std::string line;

    while (getline(data_file, line)) {
    if (line.empty()) continue;
    SimpleVector<std::string> tokens = split_line(',', line);
    cities.push_back(std::make_unique<City>(stod(tokens.at(2)), stod(tokens.at(3)), tokens.at(0), tokens.at(1)));
  }

  data_file.close();
}

/* Function that saves data for cities from the vector to disk */ 
void saveCitiesData() {
    std::ofstream data_file("cities_data.txt");

    if (cities.empty()) return;

    sortCities();

    for (auto&& item : cities) {
        std::string line = item->getCity() + "," + item->getCityCountry() + "," + std::to_string(item->getLatitude()) + "," + std::to_string(item->getLongitude()) + "\n";
        data_file << line;
    }

    data_file.close();
}

/* Function that reads the city name and coordinates from standard input and creates a new City unique_ptr in our vector */
void addCity() {
    std::string city_name;
    std::string city_country;
    double latitude;
    double longitude;

    std::cout << "Enter the name of the city: " << std::endl;
    std::getline(std::cin, city_name);

    while (!isAlphaString(city_name)) {
        std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
        std::cin.clear();
        std::getline(std::cin, city_name);
    }

    std::cout << "Enter the name of the country the city is located in: " << std::endl;
    std::getline(std::cin, city_country);

    while (!isAlphaString(city_country)) {
        std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
        std::cin.clear();
        std::getline(std::cin, city_country);
    }

    std::cout << "Enter the latitude of the city: " << std::endl;
    std::cin >> latitude;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter the longitude of the city: " << std::endl;
    std::cin >> longitude;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cities.push_back(std::make_unique<City>(latitude, longitude, city_name, city_country));
    std::cout << CLR_GREEN << "City added!" << CLR_NORMAL << std::endl;

    printProgramMenu();
    handleMenuChoice();
}

/* Function that finds a city and returns an iterator to it, If no such city is found, then the function returns cities.end() */
auto cityExists(std::string city, std::string country) {
    auto it = std::find_if(cities.begin(), cities.end(), [city, country](const std::unique_ptr<City>& city1)
    { return (city1->getCity().compare(city) == 0) && (city1->getCityCountry().compare(country) == 0); });
    return it;
}

/* Function that reads the city name from standard input and lets the user change its coordinates if the city exists in our vector
*  TODO: Let user look up a place by its coordinates instead of just city name
*/
void modifyCity() {
    std::string city_name;
    std::string city_country;
    double latitude;
    double longitude;

    std::cout << "Enter the name of the city to modify: " << std::endl;
    std::getline(std::cin, city_name);

    while (!isAlphaString(city_name)) {
        std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
        std::cin.clear();
        std::getline(std::cin, city_name);
    }

    std::cout << "Enter the name of the country the city is located in: " << std::endl;
    std::getline(std::cin, city_country);

    while (!isAlphaString(city_country)) {
        std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
        std::cin.clear();
        std::getline(std::cin, city_country);
    }

    std::cout << "Enter the new latitude of the city: " << std::endl;
    std::cin >> latitude;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter the new longitude of the city: " << std::endl;
    std::cin >> longitude;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto city = cityExists(city_name, city_country);
    
    if (city != cities.end()) {
        (*city)->setCity(latitude, longitude, city_name, city_country);
        std::cout << CLR_GREEN << "City modified!" << CLR_NORMAL << std::endl;
    } else {
        std::cout << CLR_RED << "City not found!" << CLR_NORMAL << std::endl;
    }

    printProgramMenu();
    handleMenuChoice();
}

/* Function that reads the city name from standard input and deletes it from our vector if it exists
*  TODO: Ask for confimation before deleting the city
*/
void deleteCity() {
    std::string city_name;
    std::string city_country;

    std::cout << "Enter the name of the city to delete: " << std::endl;
    std::getline(std::cin, city_name);

    while (!isAlphaString(city_name)) {
        std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
        std::cin.clear();
        std::getline(std::cin, city_name);
    }

    std::cout << "Enter the name of the country the city is located in: " << std::endl;
    std::getline(std::cin, city_country);

    while (!isAlphaString(city_country)) {
        std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
        std::cin.clear();
        std::getline(std::cin, city_country);
    }

    auto city = cityExists(city_name, city_country);

    if (city != cities.end()) {
        swap(*city, cities.back());
        cities.pop_back();
        std::cout << CLR_GREEN << "City deleted!" << CLR_NORMAL << std::endl;
    } else {
        std::cout << CLR_RED << "City not found!" << CLR_NORMAL << std::endl;
    }

    printProgramMenu();
    handleMenuChoice();
}

/* Function that prints all the cities and its coordinates that are in our vector */
void displayAllCities() {
    sortCities();

    for (auto&& item : cities) {
        std::cout << "City name: " << item->getCity() << " | City country: " << item->getCityCountry() << " | City Latitude: " << item->getLatitude() << " | City Longitude: " << item->getLongitude() << std::endl;
    }

    printProgramMenu();
    handleMenuChoice();
}

/* Function that finds the distance in kilometers between two Cities 
*  TODO: Add error handling while reading double values
*  TODO: Fix bug where having a space in the city name causes program to reject input
*/
void findDistCities() {
    std::string city_1;
    std::string city_country_1;
    std::string city_2;
    std::string city_country_2;

    std::cout << "Enter the name of the first city: " << std::endl;
    std::getline(std::cin, city_1);

    while (!isAlphaString(city_1)) {
        std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
        std::cin.clear();
        std::getline(std::cin, city_1);
    }

    std::cout << "Enter the name of the country the city is located in: " << std::endl;
    std::getline(std::cin, city_country_1);

    while (!isAlphaString(city_country_1)) {
        std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
        std::cin.clear();
        std::getline(std::cin, city_1);
    }

    std::cout << "Enter the name of the second city: " << std::endl;
    std::getline(std::cin, city_2);

    while (!isAlphaString(city_2)) {
        std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
        std::cin.clear();
        std::getline(std::cin, city_2);
    }

    std::cout << "Enter the name of the country the city is located in: " << std::endl;
    std::getline(std::cin, city_country_2);

    while (!isAlphaString(city_country_2)) {
        std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
        std::cin.clear();
        std::getline(std::cin, city_country_2);
    }

    auto city1 = cityExists(city_1, city_country_1);
    auto city2 = cityExists(city_2, city_country_2);

    if (city1 != cities.end() && city2 != cities.end()) {
        double dist = (*city1)->getDistanceTo(*city2);
        std::cout << CLR_GREEN << "Distance between the cities: " << dist << " kilometers" << CLR_NORMAL << std::endl;
    } else {
        std::cout << CLR_RED << "One of the cities you entered was not found!" << CLR_NORMAL << std::endl;
    }

    printProgramMenu();
    handleMenuChoice();
}

/* Function that saves the data, clears the vector and prints a goodbye message 
*  Exit is automatically handled as the control returns to main() which just calls return 0 and exits our program
*/
void exitProgram() {

#ifdef PRINT_PERF_STATS
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    saveCitiesData();
    std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
    std::cout << CLR_MAGENTA << "Info: Took " << duration << " microseconds to save cities data to disk" << CLR_NORMAL << std::endl;
#else
    saveCitiesData();
#endif

    cities.clear();
    std::cout << CLR_GREEN << "Goodbye!" << CLR_NORMAL << std::endl;
}

/* Function to execute the function associated with a particular menu choice using std::function magic so we don't
* have to write long if/else statements or switch cases. Each menu choice's value is linked with the std::function array's
* index, thus reducing code complexity
*/
void handleMenuChoice() {
    int choice = getProgramMenuChoice();

    std::array<std::function<void()>,6> funcs = {addCity, modifyCity, deleteCity, findDistCities, displayAllCities, exitProgram};

    clear_screen();

    if (choice >= 6) {
        std::cout << CLR_RED << "Invalid menu option, please choose a correct one" << CLR_NORMAL << std::endl;
        handleMenuChoice();
    } else {
        funcs[choice]();
    }
}