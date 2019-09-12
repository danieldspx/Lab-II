#ifndef _utils_h
#define _utils_h
    #include <string>
    #include <ctime>

    struct Dimension {
        int top;
        int right;
        int bottom;
        int left;
    };

    void pad_right(std::string &str, const unsigned int pad_size, const char pad_char = ' ');
    int get_width(Dimension dim);
    int get_height(Dimension dim);
    double get_elapsed_time(std::clock_t begin);
    int mod(int num);
#endif