#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include "editor.h"

// programa
int main(int argc, char **argv) {
    Editor editor;
    editor.inicia("saida.txt");
    editor.carrega("texto.txt");
    // editor.legenda(); //Vai ser chamada na editor.atualiza()

    while (editor.verifica_fim() == false) {
        editor.atualiza();
    }
    
    editor.salva("saida.txt");
    editor.finaliza();
    
    return 0;
}