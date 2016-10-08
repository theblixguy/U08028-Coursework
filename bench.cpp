/*
* bench.cpp
* Vector benchmarking program
*
* This program benchmarks both std::vector and SimpleVector and calculates how long a creating a vector and
* calling push_back (a very common operation done on a vector) takes. The benchmark is not 100% accurate as 
* I am not an expert at writing microbenchmarks but it should give you a fairly good idea of the performance 
* of both implementations of a vector.
*
*/

/* Imports we need */
#import <iostream>
#include <chrono>
#include <ctime>
#include <vector>
#include <random>
#include <iomanip>
#include "simple_vector.h"
#include "utils.h"
#include "city.h"

/* Number of iterations of push_back() : higher number -> more accurate benchmark */
#define NUM_OF_ITERS 100000000

/* ANSI escape codes to colorize the text */
#define CLR_NORMAL  "\x1B[0m"
#define CLR_YELLOW  "\x1B[33m"

/* To make benchmark a tiny bit more "realistic", we insert random latitudes/longitudes values 
* just to simulate real-world coordinate inputs. 
*/
std::random_device rand_dev;
std::mt19937 generator(rand_dev());
std::uniform_real_distribution<double> lat_distribution(0, 90);
std::uniform_real_distribution<double> long_distribution(0, 180);

/* Variables to hold start and end time for push_back() function */
std::clock_t cpu_start;
std::clock_t cpu_end;
std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point finish;

/* Function prototypes */
void benchmark_create_push_back();
void benchmark_std_vector_create_push_back();

/* Entry point */
int main() {

	std::cout << std::endl;
	std::cout << CLR_YELLOW << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
	std::cout << "Vector benchmaking tool (by Suyash Srijan)" << std::endl;
	std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << CLR_NORMAL << std::endl;
	std::cout << std::endl;

	std::cout << "Benchmark                    Wall Time (ns)        CPU Time (ns)           Iterations" << std::endl;
	std::cout << "--------------------------------------------------------------------------------------" << std::endl;

	std::cout << std::left << std::setw(29) << std::setfill(' ') << "SimpleVector push_back()";

	start = std::chrono::high_resolution_clock::now();
	cpu_start = std::clock();
    benchmark_create_push_back();
    cpu_end = std::clock();
    finish = std::chrono::high_resolution_clock::now();

    auto cpu_duration1 = 1e+9 * ((cpu_end - cpu_start) / CLOCKS_PER_SEC);
    auto duration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();

    std::cout << std::left << std::setw(22) << std::setfill(' ') << (duration1 / NUM_OF_ITERS);
    std::cout << std::left << std::setw(25) << std::setfill(' ') << (cpu_duration1 / NUM_OF_ITERS);
    std::cout << std::left << std::setw(0) << std::setfill(' ') << NUM_OF_ITERS;
    std::cout << std::endl;

    std::cout << std::left << std::setw(29) << std::setfill(' ') << "std::vector push_back()";

    start = std::chrono::high_resolution_clock::now();
    cpu_start = std::clock();
    benchmark_std_vector_create_push_back();
    cpu_end = std::clock();
    finish = std::chrono::high_resolution_clock::now();

    auto cpu_duration2 = 1e+9 * ((cpu_end - cpu_start) / CLOCKS_PER_SEC);
    auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();

    std::cout << std::left << std::setw(22) << std::setfill(' ') << (duration2 / NUM_OF_ITERS);
    std::cout << std::left << std::setw(25) << std::setfill(' ') << (cpu_duration2 / NUM_OF_ITERS);
    std::cout << std::left << std::setw(0) << std::setfill(' ') << NUM_OF_ITERS;
    std::cout << std::endl;
    std::cout << std::endl;

	return 0;
}

/* SimpleVector push_back() */
void benchmark_create_push_back() {
	for (int i = 0; i < NUM_OF_ITERS; i++) {
		SimpleVector<std::shared_ptr<City>> vec;
		vec.push_back(std::make_shared<City>(lat_distribution(generator), long_distribution(generator), "Test City", "Test Country"));
	}
}

/* std::vector push_back() */
void benchmark_std_vector_create_push_back() {
	for (int i = 0; i < NUM_OF_ITERS; i++) {
		std::vector<std::shared_ptr<City>> vec;
		vec.push_back(std::make_shared<City>(lat_distribution(generator), long_distribution(generator), "Test City", "Test Country"));
	}
}