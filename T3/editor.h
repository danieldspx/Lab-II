#include <vector>
#include <caca.h>

struct Dimension {
    int top;
    int right;
    int bottom;
    int left;
};

struct Editor {
    std::vector<char *> linhas;
    bool exit;
    Dimension canvas_dim;
    Dimension footer_dim;
    Dimension body_dim;

    caca_canvas_t *canvas;
    caca_display_t *display;
    caca_event_t event;

    void inicia();
    void atualiza();
    void finaliza();
    bool verifica_fim();
    void handle_events();
    void legenda();
    void update_dimensions();
}; 