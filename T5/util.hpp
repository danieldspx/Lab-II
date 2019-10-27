#pragma once

#include "structs.hpp"
#include <string>
#include <ctime>
#include <vector>

std::string position2address(Position pos);
double get_elapsed_time(std::clock_t begin);
std::vector<double> extractNumbers(std::string target);
std::vector<char> extractSymbols(std::string target);
std::vector<std::string> extractReferences(std::string target);
std::string extractSequenceSymbolNumber(std::string target);