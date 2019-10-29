#include <map>
#include <iostream>
#include <cmath>
#include <cctype>
#include <sstream>
#include <regex>
#include "planilha.hpp"
#include "celula.hpp"
#include "util.hpp"
#include "custom_exceptions.hpp"

using namespace std;

void InputBox::focus(Celula cell){
    isActive = true;
    cellRefer = cell.pos;
    input = cell.formula;
    cursor.pos.column = input.size();
}

void InputBox::defocus(){
    isActive = false;
    input = "";
    cursor.pos.column = input.size();
}

void InputBox::moveCursorRight(){
    if(cursor.pos.column+3 < dim.width){
        if(cursor.pos.column+cursorShiftX+1 <= static_cast<int>(input.size())){
           cursor.pos.column++;
        }
    } else {
        if(cursor.pos.column+cursorShiftX+1 <= static_cast<int>(input.size())){
           cursorShiftX++;
        }
    }
}

void InputBox::moveCursorLeft(){
    if(cursor.pos.column > 0){
        cursor.pos.column--;
    } else if(cursorShiftX > 0) {
        cursorShiftX--;
        if(cursorShiftX > 0){
            cursorShiftX--;
        }
    }
}

bool InputBox::isCursorAtTheEndOfString(){
    return (cursor.pos.column + cursorShiftX) == static_cast<int>(input.size());
}

void InputBox::insertChar(char letter){
    input.insert(cursor.pos.column + cursorShiftX, 1, letter);
    moveCursorRight();
}

void Planilha::init(){
    //INITIALIZE VARIABLES
    cellsDim = {width: 8, height: 3};
    cellCursor = {0,0};
    inputBox.cursor = {Position{0, 0}, true, clock()};
    shouldExit = false;
    displayShift = {0, 0};
    legendaShift = {width: 8, height: 3};
    inputBox.dim = {width: 40, height: 3};
    inputBox.isActive = false;


    // INITIALIZE CANVAS
    display = caca_create_display(NULL);
    if (!display) {
        cerr << "Erro ao criar o Display" << endl;
    }
    canvas = caca_get_canvas(display);

    caca_set_display_title(display, "Planilha");
    caca_set_color_ansi(canvas, CACA_LIGHTGREEN, CACA_BLACK);

    //INITIALIZE MAP
    for(int line = 0; line < linesSize; line++){
        for(int column = 0; column < columnsSize; column++){
            Position currentPos = {line, column};
            string address = position2address(currentPos);//convert to something like A21
            cells.insert(std::pair<std::string, Celula>(address, Celula()));//Insert the new Celula
            cells[address].init(currentPos, address,line+column);//Initialize the newly created Celula
        }
    }
}

bool Planilha::should_exit(){
    return shouldExit;
}

void Planilha::update(){
    drawCells();
    drawInputBox();
    caca_refresh_display(display);
    caca_clear_canvas(canvas);
    handle_events();
}

void Planilha::handle_events(){
    caca_get_event(display, CACA_EVENT_KEY_PRESS, &event, 100);
    switch (caca_get_event_type(&event)) {
        case CACA_EVENT_KEY_PRESS:
            handle_key_press(event);
            break;
        default:
            // cout << "Default" << endl;
            break;
    }
}

void Planilha::handle_key_press(caca_event_t ev){
    char* letter = new char;
    if(caca_get_event_type(&ev) == CACA_EVENT_KEY_PRESS){//Double check
        int caca_key = caca_get_event_key_ch(&ev);
        switch (caca_key) {
            case CACA_KEY_UP:
                if(!inputBox.isActive){
                    cursorCellUp();
                }
                break;
            case CACA_KEY_RIGHT:
                if(!inputBox.isActive){
                    cursorCellRight();
                } else {
                    inputBox.moveCursorRight();
                }
                break;
            case CACA_KEY_DOWN:
                if(!inputBox.isActive){
                    cursorCellDown();
                }
                break;
            case CACA_KEY_LEFT:
                if(!inputBox.isActive){
                    cursorCellLeft();
                } else {
                    inputBox.moveCursorLeft();
                }
                break;
            case CACA_KEY_ESCAPE:
                if(!inputBox.isActive){
                    closePlanilha();
                } else {
                    inputBox.defocus();
                }
                break;
            case CACA_KEY_RETURN:
                if(!inputBox.isActive){
                    inputBox.focus(cells.at(position2address(cellCursor)));
                } else {
                    saveInputContentOnCell();
                    inputBox.defocus();
                }
                break;
            case CACA_KEY_BACKSPACE:
                eraseChar(ERASE_BACKWARD);
                break;
            case CACA_KEY_DELETE:
                eraseChar(ERASE_FORWARD);
                break;
            // case CACA_KEY_CTRL_V:
            //     cola_linha();
            //     break;
            // case CACA_KEY_CTRL_C:
            //     copia_linha();
            //     break;
            // case CACA_KEY_RETURN:
            //     quebra_linha();
            //     break;
            // case CACA_KEY_PAUSE://SAME AS CTRL+S
            //     salva(filename_output);
            //     break;
            default:
                caca_get_event_key_utf8(&ev, letter);
                hadleCharInput(*letter);
                break;
        }
    }
    delete letter;
}

void Planilha::hadleCharInput(char letter){
    std::string target(1, letter);
    std::smatch match;
    std::regex regExp("[\\./*=^+\\-\\(\\)A-Z\\d+]");
    if(std::regex_search (target, match, regExp)){
        inputBox.insertChar(letter);
    }
}

void Planilha::cursorCellUp(){
    if(cellCursor.line > 0){
        cellCursor.line--;
        if(!isCellCursorWhitinDisplayVertical()){
            displayShift.line--;
        }
    }
};

void Planilha::cursorCellRight(){
    if(cellCursor.column+1 < columnsSize){
        cellCursor.column++;
        if(!isCellCursorWhitinDisplayHorizontal()){
            displayShift.column++;
        }
    }
};

void Planilha::cursorCellDown(){
    if(cellCursor.line+1 < linesSize){
        cellCursor.line++;
        if(!isCellCursorWhitinDisplayVertical()){
            displayShift.line++;
        }
    }
};

void Planilha::cursorCellLeft(){
    if(cellCursor.column > 0){
        cellCursor.column--;
        if(!isCellCursorWhitinDisplayHorizontal()){
            displayShift.column--;
        }
    }
};

void Planilha::drawLegenda(){
    int width = caca_get_canvas_width(canvas);
    int height = caca_get_canvas_height(canvas);

    char *valStr = new char[4];
    char asciiA = 'A';

    for(int column = 0, i = 0; hasEnoughSpaceColumn(displayShift.column + column, width) && i < columnsSize; column++, i++){
        caca_put_char(canvas, column*cellsDim.width + (cellsDim.width/2) + legendaShift.width, legendaShift.height/2, asciiA+column+displayShift.column);
    }

    for(int line = 0, i = 0; hasEnoughSpaceLine(displayShift.line + line, height) && i < linesSize; line++, i++){
        sprintf(valStr, "%d", line+displayShift.line+1);
        caca_put_str(canvas, legendaShift.width/2, line*cellsDim.height + (cellsDim.height/2) + legendaShift.height, valStr);
    }

    delete[] valStr;
}

void Planilha::drawCells(){
    int width = caca_get_canvas_width(canvas);
    int height = caca_get_canvas_height(canvas);

    string address;
    Position drawablePosition;

    drawLegenda();

    for(int line = 0, i = 0; hasEnoughSpaceLine(displayShift.line + line, height) && i < linesSize; line++, i++){
        for(int column = 0, j = 0; hasEnoughSpaceColumn(displayShift.column + column, width) && j < columnsSize; column++, j++){
            drawablePosition.line = line;//Modular reduction to fit the line in the display
            drawablePosition.column = column;
            address = position2address(Position{displayShift.line + line, displayShift.column + column});
            drawCellOnDisplay(drawablePosition, cells.at(address));
        }
    }
}

void Planilha::drawCellOnDisplay(Position pos, Celula& cell){
    char *str = new char[5];

    try{
        if(!cell.hasError) sprintf(str, "%.2lf", cell.getVal(cells));
    } catch (DepedencyException& e){
        cell._setError(cells, DepedencyException::TYPE);
        cerr << "Cell " << position2address(cell.pos) << ": Error " << e.what() << endl;
    } catch (SyntaxException& e){
        cell._setError(cells, SyntaxException::TYPE);
        cerr << "Cell " << position2address(cell.pos) << ": Error " << e.what() << endl;
    }

    if(cell.hasError){
        if(cell.raisedError == DepedencyException::TYPE){
            sprintf(str, "%s", "#RefErr"); 
        } else if(cell.raisedError == SyntaxException::TYPE){
            sprintf(str, "%s", "#SytxErr");
        }
    }
    
    pos.column = pos.column*cellsDim.width + legendaShift.width;
    pos.line = pos.line*cellsDim.height + legendaShift.height;

    if(cell.pos == cellCursor) caca_set_color_ansi(canvas, CACA_LIGHTRED, CACA_BLACK);

    caca_draw_cp437_box(canvas, pos.column, pos.line, cellsDim.width, cellsDim.height);
    caca_put_str(canvas, pos.column + 1, pos.line + cellsDim.height/2, str);

    if(cell.pos == cellCursor) caca_set_color_ansi(canvas, CACA_LIGHTGREEN, CACA_BLACK);

    delete[] str;
}

void Planilha::closePlanilha(){
    shouldExit = true;
}

bool Planilha::hasEnoughSpaceLine(int line, int height){
    height = height - (legendaShift.height + inputBox.dim.height + 3);

    return (line*cellsDim.height) < (displayShift.line*cellsDim.height+height);
}

bool Planilha::hasEnoughSpaceColumn(int column, int width){
    width = width - (legendaShift.width + 2);
    return (column*cellsDim.width) < (displayShift.column*cellsDim.width+width);
}

void Planilha::drawInputBox(){
    int height = caca_get_canvas_height(canvas);
    inputBox.dim.width = caca_get_canvas_width(canvas) - legendaShift.width;
    inputBox.dim.width = inputBox.dim.width > 0 ? inputBox.dim.width : 0;

    inputBox.pos = {line: height - inputBox.dim.height, column: legendaShift.width};

    if((inputBox.pos.line - legendaShift.height) > 0){
        caca_draw_cp437_box(canvas, inputBox.pos.column, inputBox.pos.line, inputBox.dim.width, inputBox.dim.height);
    }

    if(inputBox.isActive){
        drawInputCursor();
    } else {
        controlCursorVisibility();
    }
    drawInputContent();
}

bool Planilha::isCellCursorWhitinDisplayVertical(){
    int height = caca_get_canvas_height(canvas) - (legendaShift.height*2  + inputBox.dim.height);

    int line = cellCursor.line*cellsDim.height;

    return line >= displayShift.line*cellsDim.height && line < (displayShift.line*cellsDim.height+height);
}

bool Planilha::isCellCursorWhitinDisplayHorizontal(){
    int width = caca_get_canvas_width(canvas) - legendaShift.width;

    int column = cellCursor.column*cellsDim.width;

    return column >= displayShift.column*cellsDim.width && column < (displayShift.column*cellsDim.width+width);
}

void Planilha::drawInputCursor(){
    if(get_elapsed_time(inputBox.cursor.clock_begin) >= 0.08){
        inputBox.cursor.clock_begin = clock();//Reset time
        inputBox.cursor.showing = !inputBox.cursor.showing;
    }

    controlCursorVisibility();
    caca_gotoxy(canvas, inputBox.cursor.pos.column + inputBox.pos.column + 1, inputBox.pos.line + inputBox.dim.height / 2);
}

void Planilha::drawInputContent(){
    string substr = "";
    if(inputBox.isActive){
        substr = inputBox.input.substr(inputBox.cursorShiftX, inputBox.dim.width - 2);
        caca_put_str(canvas, inputBox.pos.column + 1, inputBox.pos.line + inputBox.dim.height / 2, substr.c_str());
    } else {
        Celula selectedCell = cells.at(position2address(cellCursor));
        if(!selectedCell.hasError){
            try{
                std::ostringstream out;
                out.precision(Celula::DOUBLE_PRECISION);
                out << "=" << std::fixed << selectedCell.getVal(cells);
                substr = out.str();
            } catch (DepedencyException& e){
                selectedCell._setError(cells, DepedencyException::TYPE);
                substr = e.what();
            } catch (SyntaxException& e){
                selectedCell._setError(cells, SyntaxException::TYPE);
                substr = e.what();
            }
        } else {
            if(selectedCell.raisedError == DepedencyException::TYPE){
                substr = DepedencyException().what();
            } else if(selectedCell.raisedError == SyntaxException::TYPE){
                substr = SyntaxException().what();
            }
        }
        caca_set_color_ansi(canvas, CACA_LIGHTRED, CACA_BLACK);
        caca_put_str(canvas, inputBox.pos.column + 1, inputBox.pos.line + inputBox.dim.height / 2, substr.c_str());
        caca_set_color_ansi(canvas, CACA_LIGHTGREEN, CACA_BLACK);
    }
}

void Planilha::controlCursorVisibility(){
    int flag_display_cursor = inputBox.cursor.showing && inputBox.isActive ? 1 : 0;
    caca_set_cursor(display, flag_display_cursor);
}

void Planilha::eraseChar(int direction){
    if(!inputBox.isActive) return;

    int cursorPosX = inputBox.cursor.pos.column + inputBox.cursorShiftX;
    string s = inputBox.input;

    if(direction == ERASE_BACKWARD){
        if(cursorPosX > 0){
            if(s.size() > 0 && cursorPosX > 0){
                s.erase(s.begin() + cursorPosX - 1);
                inputBox.input = s;
            }
        }
        inputBox.moveCursorLeft();
    } else {//ERASE_FORWARD
        if(!inputBox.isCursorAtTheEndOfString()) {
            string s = inputBox.input;
            if(s.size() > 0){
                s.erase(s.begin() + cursorPosX);
                inputBox.input = s;
            }
        }
    }
}

void Planilha::saveInputContentOnCell(){
    if(inputBox.input.at(0) != '='){
        inputBox.input.insert(0, "=");
    }
    if(inputBox.input == "="){
        inputBox.input.push_back('0');
    }
    if(inputBox.input == "+ASD984*/ASD2+62"){
        cout << "Para" << endl;
    }
    cells.at(position2address(cellCursor)).insert(inputBox.input, cells);
    inputBox.input = "";
}