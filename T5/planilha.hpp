#pragma once

#include <map>
#include <caca.h>
#include "celula.hpp"
#include "structs.hpp"

struct Planilha {
    caca_canvas_t *canvas;
    caca_display_t *display;
    caca_event_t event;

    const int linesSize = 60;
    const int columnsSize = 26;
    std::map<std::string, Celula> cells;
    Dimension cellsDim;
    Dimension legendaShift;
    Dimension inputBox;
    Position displayShift;
    Position cellCursor;
    bool shouldExit;


    void init();
    bool should_exit();
    void update();
    void handle_events();
    void handle_key_press(caca_event_t ev);
    void cursor_up();
    void cursor_right();
    void cursor_down();
    void cursor_left();
    void drawLegenda();
    void drawCells();
    void close_planilha();
    void drawCellOnDisplay(Position pos, Celula cell);
    bool hasEnoughSpaceLine(int line, int height);
    bool hasEnoughSpaceColumn(int column, int width);
    void drawInputBox();
    bool isCellCursorWhitinDisplayVertical();
    bool isCellCursorWhitinDisplayHorizontal();
};