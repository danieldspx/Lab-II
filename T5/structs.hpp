#pragma once

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