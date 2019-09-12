#ifndef _editor_h 
#define _editor_h 
    #include <vector>
    #include <caca.h>
    #include "utils.h"
    #include <ctime>

    using namespace std;

    struct Cursor {
        int x;
        int y;
        bool showing;
        clock_t clock_begin;
    };

    struct Editor {
        vector<char *> linhas;
        bool exit;
        int view_canvas_start;//Posicao de qual cada linha vai comecar ser mostrada
        Dimension canvas_dim;
        Dimension footer_dim;
        Dimension body_dim;
        Cursor cursor;

        caca_canvas_t *canvas;
        caca_display_t *display;
        caca_event_t event;

        int line_size(int line_pos);
        void inicia();
        int lines_total();
        void draw_cursor();
        void atualiza();
        void finaliza();
        void carrega(string filename);
        bool verifica_fim();
        void handle_events();
        void legenda();
        void update_dimensions();
        void insere(string &linha);
        void write_lines();
        void handle_key_press(caca_event_t ev);
        void move_esq();
        void move_dir();
        void move_cima();
        void move_baixo();
        void correct_view_canvas(int line_size);
    };
#endif