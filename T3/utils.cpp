#include "utils.h"
#include <string>
#include <ctime>

void pad_right(std::string &str, const unsigned int pad_size, const char pad_char){
    if(pad_size > str.size()){
        str.insert(str.size(), pad_size - str.size(), pad_char);
    }
}

int get_width(Dimension dim){
    return dim.right - dim.left;
}

int get_height(Dimension dim){
    return dim.bottom - dim.top;
};

double get_elapsed_time(std::clock_t begin){
    return static_cast<double>(clock() - begin) / CLOCKS_PER_SEC;
}

bool is_printable(int code){
    return 32 <= code && code <= 126;
}