#pragma once

#include "structs.hpp"
#include <string>

using namespace std;

struct Celula {
    Position pos;
    string formula;
    string address;
    double val; //Stores the result of the formula
    bool hasError;

    void init(Position posInitial, string addr, int valInitial); //Initialize a new Cell
};