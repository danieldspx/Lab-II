#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

// Estrutura  do Editor
struct Editor {
    std::vector<char *> linhas;

    void insere(std::string &linha) {
        char *newLinha = new char[linha.length() + 1];
        std::strcpy(newLinha, linha.c_str());
        linhas.push_back(newLinha);
    }

    int tamanho(void) {
        return linhas.size();
    }

    void finaliza(void) {
        for (auto &linha : linhas) {
            delete linha;
        }
    }
};  // fim do editor

// programa
int main(int argc, char **argv) {
    Editor editor;

    // editor.inicia();
    // editor.carrega("texto.txt");
    // editor.legenda();

    // while (editor.verifica_fim() == false) {
    //     editor.atualiza();
    // }
    // editor.salva("saida.txt");
    // editor.finaliza();

    return 0;
}