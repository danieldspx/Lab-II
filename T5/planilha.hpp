#pragma once

#include <map>
#include <caca.h>
#include "celula.hpp"
#include "structs.hpp"

const int ERASE_FORWARD = 1;
const int ERASE_BACKWARD = -1;

struct InputBox {
    Dimension dim;
    Position pos;
    Position cellRefer;
    Cursor cursor;
    std::string input;
    bool isActive;
    int cursorShiftX;

    void focus(Celula cell);
    void defocus();
    void moveCursorRight();
    void moveCursorLeft();
    void insertChar(char letter);
    bool isCursorAtTheEndOfString();
};

struct Planilha {
    caca_canvas_t *canvas;
    caca_display_t *display;
    caca_event_t event;

    const int linesSize = 60;
    const int columnsSize = 26;
    std::map<std::string, Celula> cells;
    Dimension cellsDim;
    Dimension legendaShift;
    InputBox inputBox;
    Position displayShift;
    Position cellCursor;
    bool shouldExit;


    void init();
    bool should_exit();
    void update();
    void handle_events();
    void handle_key_press(caca_event_t ev);
    void hadleCharInput(char letter);
    void cursorCellUp();
    void cursorCellRight();
    void cursorCellDown();
    void cursorCellLeft();
    void drawLegenda();
    void drawCells();
    void closePlanilha();
    void drawCellOnDisplay(Position pos, Celula cell);
    bool hasEnoughSpaceLine(int line, int height);
    bool hasEnoughSpaceColumn(int column, int width);
    void drawInputBox();
    bool isCellCursorWhitinDisplayVertical();
    bool isCellCursorWhitinDisplayHorizontal();
    void drawInputCursor();
    void drawInputContent();
    void controlCursorVisibility();
    void eraseChar(int direction);
};