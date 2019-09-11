#include "utils.h"
#include <string>

void padRight(std::string &str, const unsigned int pad_size, const char pad_char){
    if(pad_size > str.size()){
        str.insert(str.size(), pad_size - str.size(), pad_char);
    }
}