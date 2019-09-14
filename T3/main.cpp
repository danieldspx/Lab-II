#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include "editor.h"

// programa
int main(int argc, char **argv) {
    Editor editor;
    editor.inicia();
    editor.carrega("texto.txt");
    // editor.legenda(); //Vai ser chamada na editor.atualiza()

    while (editor.verifica_fim() == false) {
        editor.atualiza();
    }
    
    editor.salva("saida.txt");//Nao faz sentido salvar em um arquivo diferente. Fiz uma sobrecarga de funcoes pro salva
    editor.finaliza();
    
    return 0;
}