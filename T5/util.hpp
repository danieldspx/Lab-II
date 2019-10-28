#pragma once

#include "structs.hpp"
#include <string>
#include <ctime>

std::string position2address(Position pos);
double get_elapsed_time(std::clock_t begin);
