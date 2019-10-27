#include "celula.hpp"
#include <iostream>
#include <sstream>

using namespace std;

const int DOUBLE_PRECISION = 2;

void Celula::init(Position posInitial, string addr, double valInitial = 0){
    hasError = false;
    formula = "";
    address = addr;
    pos = posInitial;
    insert(valInitial);
}

void Celula::insert(double num){
    val = num;
    std::ostringstream out;
    out.precision(DOUBLE_PRECISION);
    out << "=" << std::fixed << val;
    insert(out.str());
}

void Celula::insert(string str){
    formula = str;
    //TODO: process the formula bro
}