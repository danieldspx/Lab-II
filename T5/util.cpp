#include "util.hpp"
#include "structs.hpp"
#include <string>
#include <ctime>

std::string position2address(Position pos){
    char* address = new char[3+1];//Like A21 (+1 is for the \0)
    const char asciiA = 'A';
    sprintf(address, "%c%d",(asciiA + pos.column), (pos.line+1));
    std::string addressStr(address);
    delete[] address;
    return addressStr;
}

double get_elapsed_time(std::clock_t begin){
    return static_cast<double>(clock() - begin) / CLOCKS_PER_SEC;
}