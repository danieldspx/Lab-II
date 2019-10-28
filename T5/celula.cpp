#include "celula.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>

using namespace std;

const int DOUBLE_PRECISION = 2;

void Celula::init(Position posInitial, string addr, double valInitial = 0){
    hasError = false;
    address = addr;
    pos = posInitial;
    insert(valInitial);
    isBeingCalculated = false;
    pristine = true;
}

void Celula::insert(double num){
    pristine = false;
    val = num;
    std::ostringstream out;
    out.precision(DOUBLE_PRECISION);
    out << "=" << std::fixed << val;
    formula = out.str();
}

void Celula::insert(string str, map<string, Celula>& cellsRef){
    pristine = false;
    formula = str;
    try{
        _processFormula(cellsRef);
    } catch (const char* msg){
        hasError = true;
        cerr << "Error: " << msg << endl;
    }
}

double Celula::getVal(map<string, Celula>& cellsRef){
    if(pristine) return val;

    if(!isBeingCalculated){ //Needs update
        _processFormula(cellsRef);
        if(!hasError && pristine && !isBeingCalculated){//If everything OK
            return val;
        }
    }

    if(isBeingCalculated) throw "Circular dependency";
    if(hasError) throw "Syntax error";

    return 0;
}

void Celula::_processFormula(map<string, Celula>& cellsRef){
    isBeingCalculated = true;
    pristine = false;
    calc.reset();

    string copyFormula = formula;
    string formulaSequence;

    vector<double> numbers;
    vector<char> symbols;
    vector<string> references;

    if(copyFormula.at(0) == '='){
        copyFormula.erase(0, 1);
    }

    numbers = _extractNumbers(copyFormula);
    symbols = _extractSymbols(copyFormula);
    references = _extractReferences(copyFormula);
    formulaSequence = _extractSequenceSymbolNumber(copyFormula);

    for(auto it = formulaSequence.begin(); it != formulaSequence.end(); it++){
        if(TYPE_SEQUENCE_SYMBOL.at(0) == *it){
            calc.operador(symbols.at(0));
            symbols.erase(symbols.begin());
        } else if (TYPE_SEQUENCE_NUMBER.at(0) == *it) {
            calc.operando(numbers.at(0));
            numbers.erase(numbers.begin());
        } else {
            double valOfReference = cellsRef.at(references.at(0)).getVal(cellsRef);
            calc.operando(valOfReference);
            references.erase(references.begin());
        }
    }

    if(calc.fim() == false){//Syntax erorr
        hasError = true;
        val = 0;
    } else {
        val = calc.resultado();
    }

    //Now we get the values for the references;
    isBeingCalculated = false;//TODO: Should tell those who depend on this to update
    pristine = true;
    calc.reset();
}

vector<double> Celula::_extractNumbers(string target){
    std::regex regExp("\\d+(\\.\\d+)?");

    vector<double> numbers;
    std::string::size_type sz;
    
    auto first = target.begin();
    auto last = target.end();

    std::match_results<decltype(first)> match;

    while (std::regex_search(first, last, match, regExp)){
        numbers.push_back(std::stod(match.str(),&sz));
        first = std::next(match.prefix().second, match.str().size());
    }

    return numbers;
}

vector<char> Celula::_extractSymbols(string target){
    std::regex regExp("[/*^+\\-\\(\\)]");

    vector<char> symbols;
    
    auto first = target.begin();
    auto last = target.end();

    std::match_results<decltype(first)> match;

    while (std::regex_search(first, last, match, regExp)){
        symbols.push_back(match.str().at(0));
        first = std::next(match.prefix().second, match.str().size());
    }

    return symbols;
}

vector<string> Celula::_extractReferences(string target){
    std::regex regExp("[A-Z]+\\d+");

    vector<string> references;
    
    auto first = target.begin();
    auto last = target.end();

    std::match_results<decltype(first)> match;

    while (std::regex_search(first, last, match, regExp)){
        references.push_back(match.str());
        first = std::next(match.prefix().second, match.str().size());
    }

    return references;
}

string Celula::_extractSequenceSymbolNumber(string target){
    std::regex regNumbers("\\d+(\\.\\d+)?");
    std::regex regSymbols("[/*^+\\-\\(\\)]");
    std::regex regReferences("[A-Z]+\\d+");

    target = std::regex_replace(target, regNumbers, TYPE_SEQUENCE_NUMBER);
    target = std::regex_replace(target, regReferences, TYPE_SEQUENCE_REFERENCE);
    target = std::regex_replace(target, regSymbols, TYPE_SEQUENCE_SYMBOL);

    return target;
}