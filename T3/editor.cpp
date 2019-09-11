#include "editor.h"
#include <caca.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>
#include "utils.h"

using namespace std;

// void insere(string &linha) {
//     char *newLinha = new char[linha.length() + 1];
//     strcpy(newLinha, linha.c_str());
//     linhas.push_back(newLinha);
// }

// int tamanho(void) {
//     return linhas.size();
// }

// void finaliza(void) {
//     for (auto &linha : linhas) {
//         delete linha;
//     }
// }

void Editor::inicia() {
    display = caca_create_display(NULL);
    if (!display) {
        cout << "Erro ao criar o Display" << endl;
    }
    exit = false;
    canvas = caca_get_canvas(display);
    caca_set_display_title(display, "Editor");
    caca_set_color_ansi(canvas, CACA_LIGHTGREEN, CACA_BLACK);
    update_dimensions();
}

void Editor::atualiza() {
    handle_events();
    caca_refresh_display(display);
}

void Editor::finaliza() {
    caca_free_display(display);
}

bool Editor::verifica_fim() {
    return exit;
}

void Editor::handle_events() {
    caca_get_event(display, CACA_EVENT_KEY_PRESS, &event, 100);
    // char letra;
    switch (caca_get_event_type(&event)) {
        case CACA_EVENT_KEY_PRESS:
            cout << caca_get_event_key_ch(&event) << endl;
            break;
        default:
            // cout << "Default" << endl;
            break;
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
        padRight(legendas[i], padding_size, ' ');
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