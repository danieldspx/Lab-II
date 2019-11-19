#pragma once

#include "structs.hpp"
#include "calculadora.hpp"
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Celula {

    const string TYPE_SEQUENCE_SYMBOL = "$";
    const string TYPE_SEQUENCE_NUMBER = "#";
    const string TYPE_SEQUENCE_REFERENCE = "@";
    static const int DOUBLE_PRECISION = 2;

    Position pos;
    Calculadora<double> calc;
    string formula;
    string address;
    vector<string> dependentsOfMe;
    vector<string> dependOnThem;
    double val; //Stores the result of the formula
    bool hasError;
    bool isBeingCalculated;
    bool pristine;
    int raisedError;

    void init(Position posInitial, string addr, double valInitial); //Initialize a new Cell
    void resetState();
    void insert(double num);
    void insert(string str, map<string, Celula>& cellsRef);
    void _processFormula(map<string, Celula>& cellsRef);
    double getVal(map<string, Celula>& cellsRef);
    void _insertDependent(string address);
    void _removeDependent(string address);
    void _propagateChangeOnDependentsOfMe(map<string, Celula>& cellsRef);
    void _propagateErrorOnDependentsOfMe(map<string, Celula>& cellsRef, const int errorType);
    void _shouldUpdate(map<string, Celula>& cellsRef);
    void _setError(map<string, Celula>& cellsRef, const int errorType);
    std::vector<double> _extractNumbers(std::string target);
    std::vector<char> _extractSymbols(std::string target);
    std::string _extractSequenceSymbolNumber(std::string target);
    std::vector<std::string> _extractReferences(std::string target);
};