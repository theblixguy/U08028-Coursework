# Makefile for Cities program (U08028 Coursework)
#
# Written by: Suyash Srijan
# Student number: 14076594
# Last modified: Sat 1 Oct 2016 21:12

CXX= g++
CXXFLAGS= -c -Wall -Wextra -std=c++14

all: cities

cities: main.o coordinate.o utils.o
	@echo	"Linking object files and generating executable..."
	$(CXX) main.o coordinate.o utils.o -o cities
	@echo	"Done!"

main.o: main.cpp
	@echo	"Compiling main.cpp..."
	@echo	"Note: Compiling main.cpp throws a -Wmissing-braces warning. It is actually a bug (LLVM #21629) so please ignore"
	$(CXX) $(CXXFLAGS) main.cpp
	@echo 	"Done!"

coordinate.o: coordinate.cpp coordinate.h
	@echo	"Compiling coordinate.cpp..."
	$(CXX) $(CXXFLAGS) coordinate.cpp
	@echo	"Done!"

utils.o: utils.cpp utils.h
	@echo	"Compiling utils.cpp..."
	$(CXX) $(CXXFLAGS) utils.cpp
	@echo	"Done!"

clean:
	@echo	"Removing old files..."
	rm *o cities
	@echo	"Done!"