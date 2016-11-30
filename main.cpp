/*
* main.cpp
* Cities program (U08028 Coursework)
* Written by: Suyash Srijan
* Student number: 14076594
*
* This code uses some C++ 11 and C++ 14 features like auto, std::make_shared, std::function, and lambdas
* so please don't forget to add -std=c++14 to g++ when compiling!
*
* To enable printing of performance stats (i.e time taken to sort & save/load cities data), add
* -D PRINT_STATS to g++ or add $(PRINT_STATS) to main.o target in makefile!
*
*/

/* Imports we need */
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <array>
#include <functional>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include "simple_vector.h"
#include "city.h"
#include "coordinate.h"
#include "utils.h"

/* Macros to store the date/time to show the compilation date/time in program */
#define BUILD_TIME __TIME__
#define BUILD_DATE __DATE__

/* Macros to store ANSI escape codes for text customization */
#define CLR_NORMAL  "\x1B[0m"
#define CLR_RED     "\x1B[31m"
#define CLR_YELLOW  "\x1B[33m"
#define CLR_GREEN   "\x1B[32m"
#define CLR_CYAN    "\x1B[36m"
#define CLR_MAGENTA "\x1B[35m"
#define BOLD_ON     "\x1B[1m"
#define BOLD_OFF    "\x1B[21m"

/* Define some compiler specific macros to detect whether the program was compiled using GCC, Clang,
* Microsoft Visual C++ or an unknown compiler
*/
#if defined(__clang__)
#define COMPILER_NAME "Clang"
#define COMPILER_MAJOR_VER __clang_major__
#define COMPILER_MINOR_VER __clang_minor__
#define COMPILER_PATCH_LVL __clang_patchlevel__
/* Clang/LLVM defines __GNUC__ and __GNUG__ for compatibility so check for !defined(__clang__)
* to properly detect GCC
*/
#elif defined(__GNUC__) && !defined(__clang__)
#define COMPILER_NAME "GCC"
#define COMPILER_MAJOR_VER __GNUC__
#define COMPILER_MINOR_VER __GNUC_MINOR__
#define COMPILER_PATCH_LVL __GNUC_PATCHLEVEL__
#elif defined(_MSC_VER)
#define COMPILER_NAME "MSVC"
#define COMPILER_MAJOR_VER 0
#define COMPILER_MINOR_VER 0
#define COMPILER_PATCH_LVL 0
#else
#define COMPILER_NAME "UNKNOWN"
#define COMPILER_MAJOR_VER 0
#define COMPILER_MINOR_VER 0
#define COMPILER_PATCH_LVL 0
#endif

/* Throw error if compiler isn't atleast C++14 compliant as this program uses some C++ 14 (and 11) features */
#if __cplusplus < 201402L
#error You need a C++ 14 compliant compiler to compile this program
#endif

/* Simplify shared_ptr declaration */
template <typename T>
using SPtr = typename std::shared_ptr<T>;

/* Vector of shared_ptr's pointing to our custom City class objects */
SimpleVector<SPtr<City>> cities;

/* Variable that holds the value of IsRunningWin10TH2() */
bool isRunningWin10Th2 = false;

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

/* Entry point */
int main() {
#ifdef _WIN32
    isRunningWin10Th2 = IsRunningWin10TH2();
    if (isRunningWin10Th2) {
        enableEscapeSequencesOnConsole();
    }
#endif

#ifdef PRINT_STATS
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    loadCitiesData();
    std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
#ifdef _WIN32
    if (isRunningWin10Th2) {
        std::cout << CLR_MAGENTA << "Info: Took " << duration << " milliseconds to load cities data into vector" << CLR_NORMAL << std::endl;
    } else {
        std::cout << "Info: Took " << duration << " milliseconds to load cities data into vector" << std::endl;
    }
#else
    std::cout << CLR_MAGENTA << "Info: Took " << duration << " milliseconds to load cities data into vector" << CLR_NORMAL << std::endl;
#endif
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
#ifdef _WIN32
    if (isRunningWin10Th2) {
        std::cout << CLR_CYAN << "------------------------------------------------------------" << std::endl;
        std::cout << BOLD_ON << "Cities program (U08028 Coursework) by Suyash Srijan" << std::endl;
        std::cout << "Compiled on: " << BUILD_DATE << " / " << BUILD_TIME << " using " << COMPILER_NAME << " " << COMPILER_MAJOR_VER << "." << COMPILER_MINOR_VER << "." << COMPILER_PATCH_LVL << BOLD_OFF << std::endl;
        std::cout << "------------------------------------------------------------" << CLR_NORMAL << std::endl;
    } else {
        std::cout << "------------------------------------------------------------" << std::endl;
        std::cout << "Cities program (U08028 Coursework) by Suyash Srijan" << std::endl;
        std::cout << "Compiled on: " << BUILD_DATE << " / " << BUILD_TIME << " using " << COMPILER_NAME << " " << COMPILER_MAJOR_VER << "." << COMPILER_MINOR_VER << "." << COMPILER_PATCH_LVL << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;
    }
#else
    std::cout << CLR_CYAN << "------------------------------------------------------------" << std::endl;
    std::cout << BOLD_ON << "Cities program (U08028 Coursework) by Suyash Srijan" << std::endl;
    std::cout << "Compiled on: " << BUILD_DATE << " / " << BUILD_TIME << " using " << COMPILER_NAME << " " << COMPILER_MAJOR_VER << "." << COMPILER_MINOR_VER << "." << COMPILER_PATCH_LVL << BOLD_OFF << std::endl;
    std::cout << "------------------------------------------------------------" << CLR_NORMAL << std::endl;
#endif
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

#ifdef PRINT_STATS
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::sort(cities.begin(), cities.end(), [](const SPtr<City>& city1, const SPtr<City>& city2) {
        return city1->getCity() < city2->getCity();
    });
    std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
#ifdef _WIN32
    if (isRunningWin10Th2) {
        std::cout << CLR_MAGENTA << "Info: Took " << duration << " microseconds to sort " << cities.size() << " cities" << CLR_NORMAL << std::endl;
    } else {
        std::cout << "Info: Took " << duration << " microseconds to sort " << cities.size() << " cities" << std::endl;
    }
#else
    std::cout << CLR_MAGENTA << "Info: Took " << duration << " microseconds to sort " << cities.size() << " cities" << CLR_NORMAL << std::endl;
#endif
#else
    std::sort(cities.begin(), cities.end(), [](const SPtr<City>& city1, const SPtr<City>& city2) {
        return city1->getCity() < city2->getCity();
    });
#endif

}

/* Function that loads data for cities from the disk into the vector */
void loadCitiesData() {
    std::ifstream data_file("cities_data.txt");

    if (!data_file.good()) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_YELLOW << BOLD_ON << "Warn: Database file missing or bad, a new one has been created\n" << BOLD_OFF << CLR_NORMAL << std::endl;
        } else {
            std::cout << "Warn: Database file missing or bad, a new one has been created\n" << std::endl;
        }
#else
        std::cout << CLR_YELLOW << BOLD_ON << "Warn: Database file missing or bad, a new one has been created\n" << BOLD_OFF << CLR_NORMAL << std::endl;
#endif
        std::ofstream outfile ("cities_data.txt");
        outfile.close();
        return;
    }
    std::string line;

    while (getline(data_file, line)) {
        if (line.empty()) continue;
        line.erase(remove(line.begin(), line.end(), '\"'), line.end());
        SimpleVector<std::string> tokens = split_line(',', line);
        cities.push_back(std::make_shared<City>(stod(tokens.at(2)), stod(tokens.at(3)), tokens.at(0), tokens.at(1)));
    }

    data_file.close();
}

/* Function that saves data for cities from the vector to disk */
void saveCitiesData() {
    std::ofstream data_file("cities_data.txt");

    if (cities.empty()) return;
    sortCities();

    for (auto && item : cities) {
        std::string line = item->getCity() + "," + item->getCityCountry() + "," + std::to_string(item->getLatitude()) + "," + std::to_string(item->getLongitude()) + "\n";
        data_file << line;
    }

    data_file.close();
}

/* Function that finds a city and returns an iterator to it, If no such city is found, then the function returns cities.end() */
auto cityExists(std::string const& city, std::string const& country) {
    auto it = std::find_if(cities.begin(), cities.end(), [city, country](const SPtr<City>& city1) {
        return (caseInsensitiveCompare(city1->getCity(), city)) && (caseInsensitiveCompare(city1->getCityCountry(), country));
    });
    return it;
}

/* Function that reads the city name and coordinates from standard input and creates a new City shared_ptr in our vector */
void addCity() {
    std::string city_name;
    std::string city_country;
    double latitude;
    double longitude;

    std::cout << "Enter the name of the city: " << std::endl;
    std::getline(std::cin, city_name);

    while (!isAlphaString(city_name)) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
        } else {
            std::cout << "City name cannot have numbers or any other symbols in it. Please re-enter the city name: ";
        }
#else
        std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
#endif
        std::cin.clear();
        std::getline(std::cin, city_name);
    }

    std::cout << "Enter the name of the country the city is located in: " << std::endl;
    std::getline(std::cin, city_country);

    while (!isAlphaString(city_country)) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
        } else {
            std::cout << "City country cannot have numbers or any other symbols in it. Please re-enter the city country: ";
        }
#else
        std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
#endif
        std::cin.clear();
        std::getline(std::cin, city_country);
    }

    auto city = cityExists(city_name, city_country);

    if (city != cities.end()) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City already exists!" << CLR_NORMAL << std::endl;
        } else {
            std::cout << "City already exists!" << std::endl;
        }
#else
        std::cout << CLR_RED << "City already exists!" << CLR_NORMAL << std::endl;
#endif
    }
    else {
        std::cout << "Enter the latitude of the city: " << std::endl;
        std::cin >> latitude;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter the longitude of the city: " << std::endl;
        std::cin >> longitude;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cities.push_back(std::make_shared<City>(latitude, longitude, city_name, city_country));
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_GREEN << "City added!" << CLR_NORMAL << std::endl;
        } else {
            std::cout << "City added!" << std::endl;
        }
#else
        std::cout << CLR_GREEN << "City added!" << CLR_NORMAL << std::endl;
#endif
    }

    printProgramMenu();
    handleMenuChoice();
}

/* Function that reads the city name from standard input and lets the user change its coordinates if the city exists in our vector
*  TODO: Let user look up a place by its coordinates instead of just city name
*/
void modifyCity() {
    std::string city_name;
    std::string city_country;
    std::string new_city_name;
    std::string new_country_name;
    double latitude;
    double longitude;

    std::cout << "Enter the name of the city to modify: " << std::endl;
    std::getline(std::cin, city_name);

    while (!isAlphaString(city_name)) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
        } else {
            std::cout << "City name cannot have numbers or any other symbols in it. Please re-enter the city name: ";
        }
#else
        std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
#endif
        std::cin.clear();
        std::getline(std::cin, city_name);
    }

    std::cout << "Enter the name of the country the city is located in: " << std::endl;
    std::getline(std::cin, city_country);

    while (!isAlphaString(city_country)) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
        } else {
            std::cout << "City country cannot have numbers or any other symbols in it. Please re-enter the city country: ";
        }
#else
        std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
#endif
        std::cin.clear();
        std::getline(std::cin, city_country);
    }

    auto city = cityExists(city_name, city_country);

    if (city != cities.end()) {
        std::cout << "Enter the new name of the city: " << std::endl;
        std::cin >> new_city_name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter the new name of the country: " << std::endl;
        std::cin >> new_country_name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter the new latitude of the city: " << std::endl;
        std::cin >> latitude;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter the new longitude of the city: " << std::endl;
        std::cin >> longitude;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        (*city)->setCityCoords(latitude, longitude);
        (*city)->setCityAndCountry(new_city_name, new_country_name);
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_GREEN << "City modified!" << CLR_NORMAL << std::endl;
        } else {
            std::cout << "City modified!" << std::endl;
        }
#else
        std::cout << CLR_GREEN << "City modified!" << CLR_NORMAL << std::endl;
#endif
    } else {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City not found!" << CLR_NORMAL << std::endl;
        } else {
            std::cout << "City not found!" << std::endl;
        }
#else
        std::cout << CLR_RED << "City not found!" << CLR_NORMAL << std::endl;
#endif
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
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
        } else {
            std::cout << "City name cannot have numbers or any other symbols in it. Please re-enter the city name: ";
        }
#else
        std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
#endif
        std::cin.clear();
        std::getline(std::cin, city_name);
    }

    std::cout << "Enter the name of the country the city is located in: " << std::endl;
    std::getline(std::cin, city_country);

    while (!isAlphaString(city_country)) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
        } else {
            std::cout << "City country cannot have numbers or any other symbols in it. Please re-enter the city country: ";
        }
#else
        std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
#endif
        std::cin.clear();
        std::getline(std::cin, city_country);
    }

    auto city = cityExists(city_name, city_country);

    if (city != cities.end()) {
        std::swap(*city, *cities.back());
        cities.pop_back();
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_GREEN << "City deleted!" << CLR_NORMAL << std::endl;
        } else {
            std::cout << "City deleted!" << std::endl;
        }
#else
        std::cout << CLR_GREEN << "City deleted!" << CLR_NORMAL << std::endl;
#endif
    } else {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_GREEN << "City not found!" << CLR_NORMAL << std::endl;
        } else {
            std::cout << "City not found!" << std::endl;
        }
#else
        std::cout << CLR_GREEN << "City not found!" << CLR_NORMAL << std::endl;
#endif
    }

    printProgramMenu();
    handleMenuChoice();
}

/* Function that prints all the cities and its coordinates that are in our vector in a formatted table */
void displayAllCities() {
    sortCities();

#ifdef PRINT_STATS
#ifdef _WIN32
    if (isRunningWin10Th2) {
        std::cout << CLR_MAGENTA << "Info: Program is using " << sizeof(SimpleVector<SPtr<City>>) + (sizeof(SPtr<City>) * cities.size()) << " bytes to store " << cities.size() << " items" << CLR_NORMAL << std::endl;
    } else {
        std::cout << "Info: Program is using " << sizeof(SimpleVector<SPtr<City>>) + (sizeof(SPtr<City>) * cities.size()) << " bytes to store " << cities.size() << " items" << std::endl;
    }
#else
    std::cout << CLR_MAGENTA << "Info: Program is using " << sizeof(SimpleVector<SPtr<City>>) + (sizeof(SPtr<City>) * cities.size()) << " bytes to store " << cities.size() << " items" << CLR_NORMAL << std::endl;
#endif
#endif

    std::cout << "Name                    Country                    Latitude          Longitude" << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;

    for (auto && item : cities) {
        std::cout << std::left << std::setw(24) << std::setfill(' ') << item->getCity();
        std::cout << std::left << std::setw(27) << std::setfill(' ') << item->getCityCountry();
        std::cout << std::left << std::setw(18) << std::setfill(' ') << item->getLatitude();
        std::cout << std::left << std::setw(10) << std::setfill(' ') << item->getLongitude();
        std::cout << std::endl;
    }

    printProgramMenu();
    handleMenuChoice();
}

/* Function that finds the distance in kilometers between two Cities
*  TODO: Fix bug where having a space in the city name causes program to reject input **FIXED**
*/
void findDistCities() {
    std::string city_1;
    std::string city_country_1;
    std::string city_2;
    std::string city_country_2;

    std::cout << "Enter the name of the first city: " << std::endl;
    std::getline(std::cin, city_1);

    while (!isAlphaString(city_1)) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
        } else {
            std::cout << "City name cannot have numbers or any other symbols in it. Please re-enter the city name: ";
        }
#else
        std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
#endif
        std::cin.clear();
        std::getline(std::cin, city_1);
    }

    std::cout << "Enter the name of the country the city is located in: " << std::endl;
    std::getline(std::cin, city_country_1);

    while (!isAlphaString(city_country_1)) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
        } else {
            std::cout << "City country cannot have numbers or any other symbols in it. Please re-enter the city country: ";
        }
#else
        std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
#endif
        std::cin.clear();
        std::getline(std::cin, city_1);
    }

    std::cout << "Enter the name of the second city: " << std::endl;
    std::getline(std::cin, city_2);

    while (!isAlphaString(city_2)) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
        } else {
            std::cout << "City name cannot have numbers or any other symbols in it. Please re-enter the city name: ";
        }
#else
        std::cout << CLR_RED << "City name cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city name: ";
#endif
        std::cin.clear();
        std::getline(std::cin, city_2);
    }

    std::cout << "Enter the name of the country the city is located in: " << std::endl;
    std::getline(std::cin, city_country_2);

    while (!isAlphaString(city_country_2)) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
        } else {
            std::cout << "City country cannot have numbers or any other symbols in it. Please re-enter the city country: ";
        }
#else
        std::cout << CLR_RED << "City country cannot have numbers or any other symbols in it. " << CLR_NORMAL << "Please re-enter the city country: ";
#endif
        std::cin.clear();
        std::getline(std::cin, city_country_2);
    }

    auto city1 = cityExists(city_1, city_country_1);
    auto city2 = cityExists(city_2, city_country_2);

    if (city1 != cities.end() && city2 != cities.end()) {
        double dist = (*city1)->getDistanceTo(*city2);
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_GREEN << "Distance between the cities: " << std::fixed << dist << " kilometers" << CLR_NORMAL << std::endl;
        } else {
            std::cout << "Distance between the cities: " << std::fixed << dist << " kilometers" << std::endl;
        }
#else
        std::cout << CLR_GREEN << "Distance between the cities: " << std::fixed << dist << " kilometers" << CLR_NORMAL << std::endl;
#endif
    } else {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "One of the cities you entered was not found!" << CLR_NORMAL << std::endl;
        } else {
            std::cout << "One of the cities you entered was not found!" << std::endl;
        }
#else
        std::cout << CLR_RED << "One of the cities you entered was not found!" << CLR_NORMAL << std::endl;
#endif
    }

    printProgramMenu();
    handleMenuChoice();
}

/* Function that saves the data, clears the vector and prints a goodbye message
*  Exit is automatically handled as the control returns to main() which just calls return 0 and exits our program
*/
void exitProgram() {

#ifdef PRINT_STATS
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    saveCitiesData();
    std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
#ifdef _WIN32
    if (isRunningWin10Th2) {
        std::cout << CLR_MAGENTA << "Info: Took " << duration << " milliseconds to save cities data to disk" << CLR_NORMAL << std::endl;
    } else {
        std::cout << "Info: Took " << duration << " milliseconds to save cities data to disk" << std::endl;
    }
#else
    std::cout << CLR_MAGENTA << "Info: Took " << duration << " milliseconds to save cities data to disk" << CLR_NORMAL << std::endl;
#endif
#else
    saveCitiesData();
#endif
#ifdef _WIN32
    if (isRunningWin10Th2) {
        std::cout << CLR_GREEN << "Goodbye!" << CLR_NORMAL << std::endl;
    } else {
        std::cout << "Goodbye!" << std::endl;
    }
#else
    std::cout << CLR_GREEN << "Goodbye!" << CLR_NORMAL << std::endl;
#endif
}

/* Function to execute the function associated with a particular menu choice using std::function magic so we don't
* have to write long if/else statements or switch cases. Each menu choice's value is linked with the std::function array's
* index, thus reducing code complexity
*/
void handleMenuChoice() {
    int choice = getProgramMenuChoice();

    std::array<std::function<void()>, 6> funcs = {addCity, modifyCity, deleteCity, findDistCities, displayAllCities, exitProgram};

    clear_screen();

    if (choice >= 6) {
#ifdef _WIN32
        if (isRunningWin10Th2) {
            std::cout << CLR_RED << "Invalid menu option, please choose a correct one" << CLR_NORMAL << std::endl;
        } else {
            std::cout << "Invalid menu option, please choose a correct one" << std::endl;
        }
#else
        std::cout << CLR_RED << "Invalid menu option, please choose a correct one" << CLR_NORMAL << std::endl;
#endif
        handleMenuChoice();
    } else {
        funcs[choice]();
    }
}