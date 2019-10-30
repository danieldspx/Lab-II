#include "celula.hpp"
#include "util.hpp"
#include "custom_exceptions.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

void Celula::init(Position posInitial, string addr, double valInitial = 0){
    hasError = false;
    address = addr;
    pos = posInitial;
    insert(valInitial);
    isBeingCalculated = false;
    pristine = true;
}

void Celula::resetState(){
    hasError = false;
    pristine = false;
}

void Celula::insert(double num){
    resetState();
    val = num;
    std::ostringstream out;
    out.precision(DOUBLE_PRECISION);
    out << "=" << std::fixed << val;
    formula = out.str();
}

void Celula::insert(string str, map<string, Celula>& cellsRef){
    resetState();
    formula = str;
    try{
        _processFormula(cellsRef);
    } catch (DepedencyException& e){
        _setError(cellsRef, DepedencyException::TYPE);
    } catch (SyntaxException& e){
        _setError(cellsRef, SyntaxException::TYPE);
    }
}

double Celula::getVal(map<string, Celula>& cellsRef){
    if(!hasError){
        if(pristine) return val;

        if(!isBeingCalculated && !hasError){ //Needs update (If pristine is true, it will never reach this point)
            _processFormula(cellsRef);
            if(!hasError && pristine && !isBeingCalculated){//If everything OK
                return val;
            }
        }
    }
 
    if(isBeingCalculated || raisedError == DepedencyException::TYPE){
        throw DepedencyException();
    }
    if(hasError || raisedError == SyntaxException::TYPE){
        throw SyntaxException();
    }

    return 0;
}

void Celula::_insertDependent(string address){
    if(std::find(dependentsOfMe.begin(), dependentsOfMe.end(), address) == dependentsOfMe.end()){//Does not contain address
        dependentsOfMe.push_back(address);
    }
}

void Celula::_removeDependent(string address){
    auto it = std::find(dependentsOfMe.begin(), dependentsOfMe.end(), address);
    if(it != dependentsOfMe.end()){//Contains the address
        dependentsOfMe.erase(it);
    }
}

void Celula::_processFormula(map<string, Celula>& cellsRef){
    string thisCellAddress = position2address(pos);
    isBeingCalculated = true;
    resetState();
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

    if(numbers.empty() && symbols.empty() && references.empty()){
        throw SyntaxException();
    }

    if(!dependOnThem.empty()){
        for(auto& addr: dependOnThem){//Tell cells that it does not depend on them (maybe it still depends for some of them)
            if(addr != thisCellAddress){
                cellsRef.at(addr)._removeDependent(thisCellAddress);
            }
        }
    }

    dependOnThem = references;

    for(auto& addr: dependOnThem){//Tell cells that it depend on them
        if(addr != thisCellAddress){
            try{
                cellsRef.at(addr)._insertDependent(thisCellAddress);
            } catch(std::out_of_range& e){//Meaning that the reference is wrong, therefore Syntax Error
                _setError(cellsRef, SyntaxException::TYPE);
            }
        }
    }

    _propagateChangeOnDependentsOfMe(cellsRef);

    if(!hasError){
        for(auto it = formulaSequence.begin(); it != formulaSequence.end(); it++){
            if(TYPE_SEQUENCE_SYMBOL.at(0) == *it){
                calc.operador(symbols.at(0));
                symbols.erase(symbols.begin());
            } else if (TYPE_SEQUENCE_NUMBER.at(0) == *it){
                calc.operando(numbers.at(0));
                numbers.erase(numbers.begin());
            } else if (TYPE_SEQUENCE_REFERENCE.at(0) == *it){
                try{
                    double valOfReference = cellsRef.at(references.at(0)).getVal(cellsRef);
                    calc.operando(valOfReference);
                    references.erase(references.begin());
                } catch (DepedencyException& e){
                    _setError(cellsRef, DepedencyException::TYPE);
                    break;
                } catch (SyntaxException& e){
                    _setError(cellsRef, SyntaxException::TYPE);
                    break;
                }
            } else {
                _setError(cellsRef, SyntaxException::TYPE);
            }
        }
    }

    if(!hasError){
        if(calc.fim() == false){//Syntax erorr
            _setError(cellsRef, SyntaxException::TYPE);
            val = 0;
        } else {
            val = calc.resultado();
        }
    }

    isBeingCalculated = false;
    pristine = true;
    calc.reset();
}

void Celula::_propagateChangeOnDependentsOfMe(map<string, Celula>& cellsRef){
    for(auto& addr: dependentsOfMe){
        if(cellsRef.at(addr).pristine || cellsRef.at(addr).hasError){//Oly propagate if it has no error already
            cellsRef.at(addr)._shouldUpdate(cellsRef);
        }
    }
}

void Celula::_propagateErrorOnDependentsOfMe(map<string, Celula>& cellsRef, const int errorType){
    for(auto& addr: dependentsOfMe){
        if(!cellsRef.at(addr).hasError){//Only propagate if it has no error already
            cellsRef.at(addr)._setError(cellsRef, errorType);
        }
    }
}

void Celula::_shouldUpdate(map<string, Celula>& cellsRef){
    resetState();
    _propagateChangeOnDependentsOfMe(cellsRef);
}

void Celula::_setError(map<string, Celula>& cellsRef, const int errorType){
    hasError = true;
    raisedError = errorType;
    _propagateErrorOnDependentsOfMe(cellsRef, errorType);
}

vector<double> Celula::_extractNumbers(string target){
    std::regex regExp("\\d+(\\.\\d+)?");

    vector<double> numbers;
    std::string::size_type sz;
    
    auto first = target.begin();
    auto last = target.end();

    std::match_results<decltype(first)> match;

    while (std::regex_search(first, last, match, regExp)){
        numbers.push_back(std::stod(match.str() ,&sz));
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