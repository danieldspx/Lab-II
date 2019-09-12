#include "editor.h"
#include <caca.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include "utils.h"

using namespace std;

int Editor::line_size(int line_pos) {
    string s = linhas[line_pos];
    return static_cast<int>(s.size());
}

int Editor::lines_total(){
    return static_cast<int>(linhas.size());
}

void Editor::finaliza() {
    for (auto &linha : linhas) {
        delete linha;
    }
    exit = true;
    caca_free_display(display);
    display =  nullptr;
}

void Editor::inicia() {
    display = caca_create_display(NULL);
    if (!display) {
        cout << "Erro ao criar o Display" << endl;
    }
    exit = false;
    view_canvas_start = 0;
    canvas = caca_get_canvas(display);
    cursor = {0, 0, true, clock()};

    caca_set_display_title(display, "Editor");
    caca_set_color_ansi(canvas, CACA_LIGHTGREEN, CACA_BLACK);

    update_dimensions();
}

void Editor::draw_cursor(){
    if(get_elapsed_time(cursor.clock_begin) >= 0.03){
        cursor.clock_begin = clock();//Reset time
        cursor.showing = !cursor.showing;
    }

    int flag_display_cursor = cursor.showing ? 1 : 0;
    caca_set_cursor(display, flag_display_cursor);
    caca_gotoxy(canvas, cursor.x, cursor.y);
}

void Editor::atualiza() {
    //This order is because handle_events can free display. If so, no one else should use it
    //Thats why he is the last one
    write_lines();
    draw_cursor();
    legenda();
    caca_refresh_display(display);
    caca_clear_canvas(canvas);
    handle_events();
}

bool Editor::verifica_fim() {
    return exit;
}

void Editor::handle_events() {
    caca_get_event(display, CACA_EVENT_KEY_PRESS, &event, 100);
    // char letra;
    switch (caca_get_event_type(&event)) {
        case CACA_EVENT_KEY_PRESS:
            handle_key_press(event);
            break;
        default:
            // cout << "Default" << endl;
            break;
    }
}

void Editor::handle_key_press(caca_event_t ev){
    if(caca_get_event_type(&ev) == CACA_EVENT_KEY_PRESS){//Double check
        int caca_key = caca_get_event_key_ch(&ev);
        switch (caca_key) {
            case CACA_KEY_UP:
                move_cima();
                break;
            case CACA_KEY_RIGHT:
                move_dir();
                break;
            case CACA_KEY_DOWN:
                move_baixo();
                break;
            case CACA_KEY_LEFT:
                move_esq();
                break;
            case CACA_KEY_ESCAPE:
                finaliza();
                break;
            default:
                cout<<"Sei n: "<<caca_key<<endl;
                break;
        }
    }
}

void Editor::legenda() {
    vector<string> legendas{
        "ESQ: sair do editor",
        "CTRL+s: salvar e sair",
        "ENTER: quebra linha"
    };

    int padding_size = -1;
    int padding_top = 1;
    int vector_size = static_cast<int>(legendas.size());
    
    for(string &legenda: legendas){
        if(padding_size < static_cast<int>(legenda.size())){//Gets the bigger and add 5 to its padding
            padding_size = legenda.size() + 5;
        }
    }

    for(int i = 0; i < vector_size; i++){
        pad_right(legendas[i], padding_size);
        int padding = 0;    
        if(i % 2 != 0){//Second on the Line
            padding = legendas[i - 1].size();
        }
        caca_put_str(canvas, footer_dim.left + padding, footer_dim.top + padding_top + i/2, legendas[i].c_str());
    }
}

void Editor::update_dimensions() {
    canvas_dim = {
        0,//TOP
        caca_get_canvas_width(canvas),//RIGHT
        caca_get_canvas_height(canvas),//BOTTOM
        0//LEFT
    };
    const int footer_height = 5; 
    footer_dim = {
        canvas_dim.bottom - footer_height,//TOP
        canvas_dim.right,//RIGHT
        canvas_dim.bottom,//BOTTOM
        canvas_dim.left//LEFT
    };

    body_dim = {
        canvas_dim.top,//TOP
        canvas_dim.right,//RIGHT
        footer_dim.top - 1,//BOTTOM
        canvas_dim.left//LEFT
    };
}

void Editor::carrega(string filename){
    ifstream file_input{filename};
    string read_line;
    while (std::getline(file_input, read_line)) {
      insere(read_line);
   }
}

void Editor::insere(string &line) {
    char *new_line = new char[line.length() + 1];//TODO: Tentar usar unique_ptr
    strcpy(new_line, line.c_str());
    linhas.push_back(new_line);
}

void Editor::write_lines(){ 
    int height_body = get_height(body_dim);
    for(int line_pos = 0; line_pos < height_body && line_pos < static_cast<int>(linhas.size()); line_pos++){
        if(line_size(line_pos) > view_canvas_start){
           caca_put_str(canvas, 0, line_pos, linhas[line_pos] + view_canvas_start); 
        }
    }
}

void Editor::move_esq(){
    if(cursor.x > body_dim.left){
        if(cursor.x - 1 == body_dim.left && view_canvas_start > 0){
            view_canvas_start--;
        } else {
            cursor.x--;
        }
    } else {
        cursor.y--;//Go to line bottom
        correct_view_canvas(line_size(cursor.y));
        cursor.x = line_size(cursor.y) - view_canvas_start;
    }
}

void Editor::move_dir(){
    if(cursor.x < body_dim.right && cursor.x + view_canvas_start < line_size(cursor.y)){
        if(cursor.x + 1 == body_dim.right){
            //Do not incremet because the cursor is about to get off canvas
            //and it is correct to not increment because it should stay where it is
            //and just the view canvas show move
            view_canvas_start++;
        } else {
            cursor.x++;
        }
    } else {//Is trying to go right and is in the end of the string, so we go down the line
        cursor.x = 0;
        cursor.y++;//Go to line bottom
        view_canvas_start = 0;
    }
}

void Editor::move_cima(){
    if(cursor.y > body_dim.top){
        int above_line_size = line_size(cursor.y - 1);
        if((view_canvas_start + cursor.x) > above_line_size){
            correct_view_canvas(above_line_size);
            cursor.x = above_line_size - view_canvas_start;
        }
        cursor.y--;
    }
}

void Editor::move_baixo(){
    if(cursor.y < body_dim.bottom && cursor.y < lines_total()){
        int below_line_size = line_size(cursor.y + 1);
        if((view_canvas_start + cursor.x) > below_line_size){
            correct_view_canvas(below_line_size);
            cursor.x = below_line_size - view_canvas_start;
        }
        cursor.y++;
    }
}

void Editor::correct_view_canvas(int line_size){
    if(line_size < view_canvas_start || line_size > view_canvas_start+get_width(body_dim)){
        view_canvas_start = line_size - get_width(body_dim) + 1;
        if(view_canvas_start < 0){
            view_canvas_start = 0;
        }
    }
}