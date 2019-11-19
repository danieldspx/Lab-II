#include <iostream>
#include "planilha.hpp"
#include "util.hpp"
#include "structs.hpp"

using namespace std;

// void handle_events() {
//     caca_get_event(display, CACA_EVENT_KEY_PRESS, &event, 100);
//     switch (caca_get_event_type(&event)) {
//         case CACA_EVENT_KEY_PRESS:
//             // handle_key_press(event);
//             shouldExit = true;
//             break;
//         default:
//             // cout << "Default" << endl;
//             break;
//     }
// }

int main(){

    Planilha planilha;

    //Enter = Entra no modo de edicao da celula -- Se estiver na celula ele salva o valor
    //Q = Sai do modo de edicao da celula


    planilha.init();
    while(!planilha.should_exit()){
        planilha.update();
    }

    return 0;
}