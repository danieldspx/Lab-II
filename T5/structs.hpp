#pragma once

#include <ctime>

struct Position {
    int line;
    int column;

    bool operator==(const Position& pos) {
        return (line == pos.line && column == pos.column);
    }
};

struct Dimension {
    int width;
    int height;
};

struct Cursor {
    Position pos;
    bool showing;
    clock_t clock_begin;
};
