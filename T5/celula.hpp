#pragma once

#include "structs.hpp"
#include "calculadora.hpp"
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Celula {

    const string TYPE_SEQUENCE_SYMBOL = "0";
    const string TYPE_SEQUENCE_NUMBER = "1";
    const string TYPE_SEQUENCE_REFERENCE = "2";

    Position pos;
    Calculadora<double> calc;
    string formula;
    string address;
    double val; //Stores the result of the formula
    bool hasError;
    bool isBeingCalculated;
    bool pristine;

    void init(Position posInitial, string addr, double valInitial); //Initialize a new Cell
    void insert(double num);
    void insert(string str, map<string, Celula>& cellsRef);
    void _processFormula(map<string, Celula>& cellsRef);
    double getVal(map<string, Celula>& cellsRef);
    std::vector<double> _extractNumbers(std::string target);
    std::vector<char> _extractSymbols(std::string target);
    std::string _extractSequenceSymbolNumber(std::string target);
    std::vector<std::string> _extractReferences(std::string target);
};