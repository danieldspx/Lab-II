#include <map>
#include <iostream>
#include <cmath>
#include "planilha.hpp"
#include "celula.hpp"
#include "util.hpp"

using namespace std;

void Planilha::init(){
    //INITIALIZE VARIABLES
    cellsDim = {width: 8, height: 3};
    cursor = {0,0};
    shouldExit = false;
    displayShift = {0, 0};
    legendaShift = cellsDim;
    inputBox = {width: 40, height: 3};

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
            cells[address].init(currentPos, address);//Initialize the newly created Celula
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
    if(caca_get_event_type(&ev) == CACA_EVENT_KEY_PRESS){//Double check
        int caca_key = caca_get_event_key_ch(&ev);
        switch (caca_key) {
            case CACA_KEY_UP:
                cursor_up();
                break;
            case CACA_KEY_RIGHT:
                cursor_right();
                break;
            case CACA_KEY_DOWN:
                cursor_down();
                break;
            case CACA_KEY_LEFT:
                cursor_left();
                break;
            case CACA_KEY_ESCAPE:
                close_planilha();
                break;
            // case CACA_KEY_BACKSPACE:
            //     erase_char(ERASE_BACKWARD);
            //     break;
            // case CACA_KEY_DELETE:
            //     erase_char(ERASE_FORWARD);
            //     break;
            // case CACA_KEY_CTRL_X:
            //     recorta_linha();
            //     break;
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
                cout << "Default " << (char)caca_key << endl;
                // if(is_printable(caca_key)) {
                //     insere_char(caca_key);
                // }
                // break;
        }
    }
}

void Planilha::cursor_up(){};

void Planilha::cursor_right(){};

void Planilha::cursor_down(){};

void Planilha::cursor_left(){};

void Planilha::drawLegenda(){
    int width = caca_get_canvas_width(canvas);
    int height = caca_get_canvas_height(canvas);

    char *valStr = new char[4];
    char asciiA = 'A';

    for(int column = displayShift.column, i = 0; hasEnoughSpaceColumn(column, width) && i < columnsSize; column++, i++){
        caca_put_char(canvas, column*cellsDim.width + (cellsDim.width/2) + legendaShift.width, legendaShift.height/2, asciiA+column);
    }

    for(int line = displayShift.line, i = 0; hasEnoughSpaceLine(line, height) && i < linesSize; line++, i++){
        sprintf(valStr, "%d", line+1);
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

    for(int line = displayShift.line, i = 0; hasEnoughSpaceLine(line, height) && i < linesSize; line++, i++){
        for(int column = displayShift.column, j = 0; hasEnoughSpaceColumn(column, width) && j < columnsSize; column++, j++){
            drawablePosition.line = line%height;//Modular reduction to fit the line in the display
            drawablePosition.column = column%width;
            address = position2address(Position{line, column});

            drawCellOnDisplay(drawablePosition, cells[address]);
        }
    }
}

void Planilha::drawCellOnDisplay(Position pos, Celula cell){
    char *str = new char[2];
    sprintf(str, "%.0lf", cell.val);
    pos.column = pos.column*cellsDim.width + legendaShift.width;
    pos.line = pos.line*cellsDim.height + legendaShift.height;
    caca_draw_cp437_box(canvas, pos.column, pos.line, cellsDim.width, cellsDim.height);
    caca_put_str(canvas, pos.column + 1, pos.line + cellsDim.height/2, str);

    delete[] str;
}

void Planilha::close_planilha(){
    shouldExit = true;
}

bool Planilha::hasEnoughSpaceLine(int line, int height){
    return (line*cellsDim.height + (int)ceil(legendaShift.height*2) - 1) < (displayShift.line+height - inputBox.height);
}

bool Planilha::hasEnoughSpaceColumn(int column, int width){
    return (column*cellsDim.width + (int)ceil(legendaShift.width*2) - 1) < (displayShift.column+width);
}

void Planilha::drawInputBox(){
    int height = caca_get_canvas_height(canvas);
    inputBox.width = caca_get_canvas_width(canvas) - legendaShift.width;
    inputBox.width = inputBox.width > 0 ? inputBox.width : 0;

    Position drawPos = {line: height - inputBox.height, column: legendaShift.width};

    if((drawPos.line - legendaShift.height) > 0){
        caca_draw_cp437_box(canvas, drawPos.column, drawPos.line, inputBox.width, inputBox.height);
    }
}

// TODO: Input box, criar a variavel pra ter a posicao da celula selecionada
// e colorir ela. Tbm tem q tratar o texto dentor da inputBox
