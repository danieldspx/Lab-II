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

    void init(Position posInitial, string addr, double valInitial); //Initialize a new Cell
    void insert(double num);
    void insert(string str);
};