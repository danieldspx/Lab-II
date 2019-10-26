#include "util.hpp"
#include "structs.hpp"
#include <string>

std::string position2address(Position pos){
    char* address = new char[3+1];//Like A21 (+1 is for the \0)
    const char asciiA = 'A';
    sprintf(address, "%c%d",(asciiA + pos.column), (pos.line+1));
    delete[] address;
    return std::string(address);
}