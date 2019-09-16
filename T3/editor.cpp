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
    for (int i = 0; i < lines_total(); i++) {
        delete_from_vector(i);
    }
    caca_free_display(display);
    display =  nullptr;
}

void Editor::end_editor(){
    exit = true;
}

void Editor::inicia(string output_file) {
    display = caca_create_display(NULL);
    if (!display) {
        cout << "Erro ao criar o Display" << endl;
    }
    filename_output = output_file;
    is_transer_area_empty = true;
    exit = false;
    has_unsaved_work = false;
    view_canvas_start = view_canvas_vertical_start = 0;
    canvas = caca_get_canvas(display);
    cursor = {0, 0, true, clock()};

    caca_set_display_title(display, "Editor");
    caca_set_color_ansi(canvas, CACA_LIGHTGREEN, CACA_BLACK);

    update_dimensions();
}

void Editor::draw_cursor(){
    if(get_elapsed_time(cursor.clock_begin) >= 0.08){
        cursor.clock_begin = clock();//Reset time
        cursor.showing = !cursor.showing;
    }

    int flag_display_cursor = cursor.showing ? 1 : 0;
    caca_set_cursor(display, flag_display_cursor);
    caca_gotoxy(canvas, cursor.x, cursor.y);
    check_cursor();
}

void Editor::atualiza() {
    write_lines();
    update_title();
    draw_cursor();
    legenda();
    caca_refresh_display(display);
    caca_clear_canvas(canvas);
    handle_events();
}

bool Editor::verifica_fim() {
    return exit;
}

bool Editor::is_cursor_end_string(){
    return (cursor.x + view_canvas_start) == line_size(real_cursor_y());
}

void Editor::handle_events() {
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

void Editor::insere_char(char c){
    string s;
    if(lines_total() != 0){
        s = linhas[real_cursor_y()];
        delete_from_vector(real_cursor_y());
        s.insert(cursor.x+view_canvas_start, 1, c);
    } else {
        s = c;
    }
    insert_in(s, real_cursor_y());
    move_dir();
}

void Editor::gruda_linha(){
    if(real_cursor_y()+1 >= lines_total()){
        return;
    }

    string current_line = linhas.at(real_cursor_y());    
    string below_line = linhas.at(real_cursor_y()+1);
    string combination_line = current_line+below_line;
    delete_from_vector(real_cursor_y()+1);//This order is important
    delete_from_vector(real_cursor_y());
    insert_in(combination_line, real_cursor_y());
}

void Editor::erase_char(int direction){
    int cursor_real_x = cursor.x + view_canvas_start; 
    if(direction == ERASE_BACKWARD){
        if(cursor_real_x > 0){
            string s = linhas[real_cursor_y()];
            if(s.size() > 0 && cursor_real_x > 0){
                delete_from_vector(real_cursor_y());
                s.erase(s.begin() + cursor_real_x - 1);
                insert_in(s, real_cursor_y());
            }
        }
        move_esq();
        if(is_cursor_end_string()){
            gruda_linha();
        }
    } else {//ERASE_FORWARD
        if(is_cursor_end_string()){
            gruda_linha();
        } else {
            string s = linhas[real_cursor_y()];
            if(s.size() > 0){
                delete_from_vector(real_cursor_y());
                s.erase(s.begin() + cursor_real_x);
                insert_in(s, real_cursor_y());
            }
        }
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
                end_editor();
                break;
            case CACA_KEY_BACKSPACE:
                erase_char(ERASE_BACKWARD);
                break;
            case CACA_KEY_DELETE:
                erase_char(ERASE_FORWARD);
                break;
            case CACA_KEY_CTRL_X:
                recorta_linha();
                break;
            case CACA_KEY_CTRL_V:
                cola_linha();
                break;
            case CACA_KEY_CTRL_C:
                copia_linha();
                break;
            case CACA_KEY_RETURN:
                quebra_linha();
                break;
            case CACA_KEY_PAUSE://SAME AS CTRL+S
                salva(filename_output);
                break;
            default:
                if(is_printable(caca_key)) {
                    insere_char(caca_key);
                }
                break;
        }
    }
}

void Editor::legenda() {
    vector<string> legendas{
        "ESQ: Sair do editor",
        "CTRL+S: Salvar",
        "ENTER: Quebra linha",
        "CTRL+X: Recortar ",
        "CTRL+C: Copiar ",
        "CTRL+V: Colar ",
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
   file_input.close();
   Editor::filename_open = filename;
   has_unsaved_work = false;
}

void Editor::insere(string &line) {
    char *new_line = new char[line.length() + 1];//TODO: Tentar usar unique_ptr
    strcpy(new_line, line.c_str());
    linhas.push_back(new_line);
    has_unsaved_work = true;
}

void Editor::insert_in(string &line, int pos){
    char *new_line = new char[line.length() + 1];
    strcpy(new_line, line.c_str());
    linhas.insert(linhas.begin() + pos, new_line);
    has_unsaved_work = true;
}

void Editor::write_lines(){ 
    int height_body = get_height(body_dim);
    for(int line_pos = view_canvas_vertical_start; (line_pos - view_canvas_vertical_start) < height_body && line_pos < lines_total(); line_pos++){
        if(line_size(line_pos) > view_canvas_start){
            int relative_y = line_pos - view_canvas_vertical_start;//We have to discount the vertical_start
           caca_put_str(canvas, 0, relative_y, linhas[line_pos] + view_canvas_start); 
        }
    }
}

bool Editor::move_esq(){
    bool has_moved = false;
    if(cursor.x > body_dim.left){
        if(cursor.x - 1 == body_dim.left && view_canvas_start > 0){
            view_canvas_start--;
        } else {
            cursor.x--;
        }
        has_moved = true;
    } else {
        if(move_cima()){
            correct_view_canvas(line_size(real_cursor_y()));
            cursor.x = line_size(real_cursor_y()) - view_canvas_start;
            has_moved = true;
        }
    }
    return has_moved;
}

bool Editor::move_dir(){
    bool has_moved = false;
    if(cursor.x < body_dim.right && cursor.x + view_canvas_start < line_size(real_cursor_y())){
        if(cursor.x + 1 == body_dim.right){
            //Do not incremet because the cursor is about to get off canvas
            //and it is correct to not increment because it should stay where it is
            //and just the view canvas show move
            view_canvas_start++;
        } else {
            cursor.x++;
        }
        has_moved = true;
    } else {//Is trying to go right and is in the end of the string, so we go down the line
        if(move_baixo()){
            cursor.x = 0;
            view_canvas_start = 0;
            has_moved = true;
        }
    }
    return has_moved;
}

bool Editor::move_cima(){
    if(cursor.y > body_dim.top){
        int above_line_size = line_size(real_cursor_y() - 1);
        if((view_canvas_start + cursor.x) > above_line_size){
            correct_view_canvas(above_line_size);
            cursor.x = above_line_size - view_canvas_start;
        }
        cursor.y--;

        return true;
    } else if (cursor.y == body_dim.top && view_canvas_vertical_start > 0) {
        view_canvas_vertical_start--;
        cursor.x = line_size(real_cursor_y());
    }

    return false;
}

bool Editor::move_baixo(){
    if(cursor.y+1 < body_dim.bottom && real_cursor_y()+1 < lines_total()){
        int below_line_size = line_size(real_cursor_y() + 1);
        if((view_canvas_start + cursor.x) > below_line_size){
            correct_view_canvas(below_line_size);
            cursor.x = below_line_size - view_canvas_start;
        }
        cursor.y++;
        return true;
    } else if (real_cursor_y()+1 < lines_total()){
        view_canvas_vertical_start++;
    }

    return false;
}

void Editor::correct_view_canvas(int line_size){
    if(line_size < view_canvas_start || line_size > view_canvas_start+get_width(body_dim)){
        view_canvas_start = line_size - get_width(body_dim) + 1;
        if(view_canvas_start < 0){
            view_canvas_start = 0;
        }
    }
}

void Editor::delete_from_vector(int pos){
    delete[] linhas[pos];
    linhas.erase(linhas.begin() + pos);
}

void Editor::copy_line_2_transfer_area(){
    transfer_area = linhas.at(real_cursor_y());
}

void Editor::recorta_linha(){
    if(lines_total() == 0){
        return;
    }

    is_transer_area_empty = false;
    has_unsaved_work = true;
    copy_line_2_transfer_area();
    delete_from_vector(real_cursor_y());
    check_cursor();
}

void Editor::cola_linha(){
    if(!is_transer_area_empty){
        insert_in(transfer_area, real_cursor_y()+1);
    }
}
void Editor::copia_linha(){
    is_transer_area_empty = false;
    copy_line_2_transfer_area();
}

void Editor::quebra_linha(){
    string cursor_line = linhas.at(real_cursor_y());
    string before_cursor = cursor_line.substr(0, cursor.x);
    string after_cursor = cursor_line.substr(cursor.x);
    delete_from_vector(real_cursor_y());
    insert_in(before_cursor, real_cursor_y());
    insert_in(after_cursor, real_cursor_y()+1);
}

void Editor::update_title(){
    if(has_unsaved_work){
        caca_set_display_title(display, "(unsaved) Editor");
    } else {
        caca_set_display_title(display, "Editor");
    }
}

void Editor::salva(string filename){
    has_unsaved_work = false;
    ofstream file{filename};
    if(!file.is_open()){
        cout << "Error opening file: " << filename << endl;
        return;
    }
    write_on_file(file);
    file.close();
}

void Editor::write_on_file(ofstream& file){
    if(!file.is_open()){
        cout << "Error on writing on file. File is not open." << endl;
        return;
    }

    for(auto& linha: linhas){
        file << linha << endl;
    }
}

int Editor::real_cursor_y(){
    return cursor.y + view_canvas_vertical_start;
}

void Editor::check_cursor(){
    if(real_cursor_y()+1 > lines_total()){
        move_cima();
    }
    if((cursor.x + view_canvas_start) > line_size(real_cursor_y())){
        cursor.x = line_size(real_cursor_y()) - view_canvas_start;
    }
}
