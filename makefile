# Makefile for Cities program (U08028 Coursework)
#
# Written by: Suyash Srijan
# Student number: 14076594

CXX= clang++
CXXFLAGS= -c -Wall -Wextra -Ofast -std=c++14
PRINT_STAT_FLAG= -D PRINT_STATS

all: cities

cities: main.o city.o coordinate.o utils.o
	@echo	"Linking object files and generating executable..."
	$(CXX) main.o city.o coordinate.o utils.o -o cities
	@echo	"Done!"

main.o: main.cpp simple_vector.h
	@echo	"Compiling main.cpp..."
	@echo	"Note: Compiling main.cpp may throw a -Wmissing-braces warning. It is actually a bug (LLVM #21629)."
	@echo	"You can suppress it by adding -Wno-missing-braces to CXXFLAGS"
	$(CXX) $(PRINT_STAT_FLAG) $(CXXFLAGS) main.cpp
	@echo 	"Done!"

coordinate.o: coordinate.cpp coordinate.h
	@echo	"Compiling coordinate.cpp..."
	$(CXX) $(CXXFLAGS) coordinate.cpp
	@echo	"Done!"

city.o: city.cpp city.h
	@echo	"Compiling city.cpp..."
	$(CXX) $(CXXFLAGS) city.cpp
	@echo	"Done!"

utils.o: utils.cpp utils.h simple_vector.h
	@echo	"Compiling utils.cpp..."
	$(CXX) $(CXXFLAGS) utils.cpp
	@echo	"Done!"

clean:
	@echo	"Removing old files..."
	rm *o cities
	@echo	"Done!"