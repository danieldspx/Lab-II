#include <caca.h>
#include <iostream>

using namespace std;

caca_canvas_t *canvas;
caca_display_t *display;
caca_event_t event;
bool shouldExit = false;

void handle_events() {
    caca_get_event(display, CACA_EVENT_KEY_PRESS, &event, 100);
    switch (caca_get_event_type(&event)) {
        case CACA_EVENT_KEY_PRESS:
            // handle_key_press(event);
            shouldExit = true;
            break;
        default:
            // cout << "Default" << endl;
            break;
    }
}

int main(){

    display = caca_create_display(NULL);
    if (!display) {
        cout << "Erro ao criar o Display" << endl;
    }
    canvas = caca_get_canvas(display);

    caca_set_display_title(display, "Editor");
    caca_set_color_ansi(canvas, CACA_LIGHTGREEN, CACA_BLACK);

    while(!shouldExit){
        caca_draw_cp437_box(canvas, 10, 5, 8, 3);
        caca_refresh_display(display);
        caca_clear_canvas(canvas);
        handle_events();
    }

    return 0;
}