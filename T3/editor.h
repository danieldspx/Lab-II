#ifndef _editor_h 
#define _editor_h 
    #include <vector>
    #include <caca.h>
    #include "utils.h"
    #include <ctime>

    using namespace std;

    const int ERASE_FORWARD = 1;
    const int ERASE_BACKWARD = -1;

    struct Cursor {
        int x;
        int y;
        bool showing;
        clock_t clock_begin;
    };

    struct Editor {
        vector<char *> linhas;
        bool exit;
        int view_canvas_vertical_start;
        int view_canvas_start;//Posicao de qual cada linha vai comecar ser mostrada
        bool has_unsaved_work;
        bool is_transer_area_empty;
        string filename_open;
        string transfer_area;
        Dimension canvas_dim;
        Dimension footer_dim;
        Dimension body_dim;
        Cursor cursor;

        caca_canvas_t *canvas;
        caca_display_t *display;
        caca_event_t event;

        int line_size(int line_pos);
        void inicia();
        void write_on_file(ofstream& file);
        void salva();
        void salva(string filename);
        int lines_total();
        void draw_cursor();
        void atualiza();
        void finaliza();
        void carrega(string filename);
        bool verifica_fim();
        void handle_events();
        bool is_cursor_end_string();
        void legenda();
        void update_dimensions();
        void insere(string &linha);
        void write_lines();
        void handle_key_press(caca_event_t ev);
        bool move_esq();
        bool move_dir();
        bool move_cima();
        bool move_baixo();
        void end_editor();
        void insere_char(char c);
        void erase_char(int direction);
        void insert_in(string &line, int pos);
        void correct_view_canvas(int line_size);
        void delete_from_vector(int pos);
        void gruda_linha();
        void copy_line_2_transfer_area();
        void recorta_linha();
        void cola_linha();
        void quebra_linha();
        void update_title();
        void copia_linha();
        int real_cursor_y();
        void check_cursor();
    };
#endif