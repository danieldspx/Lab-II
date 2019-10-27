#include "util.hpp"
#include "structs.hpp"
#include <string>
#include <ctime>
#include <vector>
#include <regex>

using namespace std;

std::string position2address(Position pos){
    char* address = new char[3+1];//Like A21 (+1 is for the \0)
    const char asciiA = 'A';
    sprintf(address, "%c%d",(asciiA + pos.column), (pos.line+1));
    std::string addressStr(address);
    delete[] address;
    return addressStr;
}

double get_elapsed_time(std::clock_t begin){
    return static_cast<double>(clock() - begin) / CLOCKS_PER_SEC;
}

vector<double> extractNumbers(string target){
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

vector<char> extractSymbols(string target){
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

vector<string> extractReferences(string target){
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

string extractSequenceSymbolNumber(string target){
    std::regex regNumbers("\\d+(\\.\\d+)?");
    std::regex regSymbols("[/*^+\\-\\(\\)]");
    std::regex regReferences("[A-Z]+\\d+");

    const string TYPE_SEQUENCE_SYMBOL = "0";
    const string TYPE_SEQUENCE_NUMBER = "1";
    const string TYPE_SEQUENCE_REFERENCE = "2";

    target = std::regex_replace(target, regNumbers, TYPE_SEQUENCE_NUMBER);
    target = std::regex_replace(target, regReferences, TYPE_SEQUENCE_REFERENCE);
    target = std::regex_replace(target, regSymbols, TYPE_SEQUENCE_SYMBOL);

    return target;
}