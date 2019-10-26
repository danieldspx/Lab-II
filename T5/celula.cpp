#include "celula.hpp"
#include <iostream>

using namespace std;

void Celula::init(Position posInitial, string addr, int valInitial = 0){
    hasError = false;
    formula = "";
    address = addr;
    pos = posInitial;
    val = valInitial;
}