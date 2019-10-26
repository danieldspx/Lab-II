#include "celula.hpp"

using namespace std;

void Celula::init(Position posInitial, string addr){
    hasError = false;
    formula = "";
    address = addr;
    pos = posInitial;
    val = 0;
}